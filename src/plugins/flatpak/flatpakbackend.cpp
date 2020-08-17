/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include <QtConcurrentRun>
#include <QFileInfo>
#include <QFutureWatcher>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QStandardPaths>
#include <QDateTime>
#include <QTime>
#include <QUuid>

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/SoftwareManager>

#include <AppStreamQt/icon.h>
#include <AppStreamQt/metadata.h>

#include "flatpakappstreamjob.h"
#include "flatpakplugin.h"
#include "flatpaksource.h"

#include <sys/stat.h>

Q_LOGGING_CATEGORY(lcFlatpakBackend, "liri.appcenter.flatpak")

using namespace Liri::AppCenter;

static void flatpakUserChanged(GFileMonitor *monitor, GFile *child, GFile *other_file,
                               GFileMonitorEvent event_type, gpointer userData)
{
    Q_UNUSED(monitor);
    Q_UNUSED(child);
    Q_UNUSED(other_file);
    Q_UNUSED(event_type);

    FlatpakBackend *backend = static_cast<FlatpakBackend *>(userData);
    if (!backend)
        return;
    backend->checkForUpdates();
}

static void flatpakSystemChanged(GFileMonitor *monitor, GFile *child, GFile *other_file,
                                 GFileMonitorEvent event_type, gpointer userData)
{
    Q_UNUSED(monitor);
    Q_UNUSED(child);
    Q_UNUSED(other_file);
    Q_UNUSED(event_type);

    FlatpakBackend *backend = static_cast<FlatpakBackend *>(userData);
    if (!backend)
        return;
    backend->checkForUpdates();
}

FlatpakBackend::FlatpakBackend(QObject *parent)
    : Liri::AppCenter::Backend(parent)
    , m_cancellable(g_cancellable_new())
{
}

FlatpakBackend::~FlatpakBackend()
{
    while (!m_installationMonitors.isEmpty())
        g_object_unref(m_installationMonitors.takeFirst());

    while (!m_installations.isEmpty())
        g_object_unref(m_installations.takeFirst());

    g_object_unref(m_cancellable);
}

void FlatpakBackend::initialize(Liri::AppCenter::SoftwareManager *manager)
{
    /* Override the umask to 022 to make it possible to share files between
     * the liri-appcenter process and flatpak system helper process.
     * Ideally this should be set when needed in the flatpak plugin, but
     * umask is thread-unsafe so there is really no local way to fix this.
     *
     * See https://github.com/flatpak/flatpak/pull/2856/
     */
    umask(022);

    Q_ASSERT(manager);
    m_manager = manager;

    g_autoptr(GError) error = nullptr;

    GPtrArray *installations = flatpak_get_system_installations(m_cancellable, &error);
    if (!installations) {
        qCWarning(lcFlatpakBackend,
                  "Unable to list system installations: %s", error->message);
    }
    for (uint i = 0; installations && i < installations->len; i++) {
        auto system = FLATPAK_INSTALLATION(g_ptr_array_index(installations, i));
        m_installations.append(system);

        auto monitor = flatpak_installation_create_monitor(system, m_cancellable, &error);
        if (monitor) {
            g_signal_connect(monitor, "changed", G_CALLBACK(flatpakSystemChanged), this);
            m_installationMonitors.append(monitor);
        } else {
            qCWarning(lcFlatpakBackend,
                      "Unable to create a monitor for system-wide installation: %s",
                      error->message);
        }
    }

    auto user = flatpak_installation_new_user(m_cancellable, &error);
    if (user) {
        m_userInstallation = user;
        m_installations.append(user);

        auto monitor = flatpak_installation_create_monitor(user, m_cancellable, &error);
        if (monitor) {
            g_signal_connect(monitor, "changed", G_CALLBACK(flatpakUserChanged), this);
            m_installationMonitors.append(monitor);
        } else {
            qCWarning(lcFlatpakBackend,
                      "Unable to create a monitor for the current user installation: %s",
                      error->message);
        }
    } else {
        qCWarning(lcFlatpakBackend,
                  "Unable to find Flatpak installation for the current user: %s",
                  error->message);
    }
}

void FlatpakBackend::listSources()
{
    // List sources
    for (auto installation : qAsConst(m_installations))
        extractRepositories(installation);
}

void FlatpakBackend::listAvailableApps()
{
    for (auto installation : qAsConst(m_installations))
        listAvailableApps(installation);
}

void FlatpakBackend::listInstalledApps()
{
    for (auto installation : qAsConst(m_installations))
        listInstalledApps(installation);
}

void FlatpakBackend::checkForUpdates()
{
    for (auto installation : m_installations) {
        checkLocalUpdatesForInstallation(installation);
        checkUpdatesForInstallation(installation);
    }
}

QVector<Liri::AppCenter::SoftwareResource *> FlatpakBackend::updates() const
{
    QVector<SoftwareResource *> list;

    for (auto resource : m_resources) {
        if (resource->type() == SoftwareResource::App && resource->state() == SoftwareResource::UpgradableState)
            list.append(resource);
    }

    return list;
}

bool FlatpakBackend::addSource(const QString &source)
{
    const QUrl url = QUrl::fromUserInput(source);

    if (!url.fileName().endsWith(QLatin1String(".flatpakrepo")))
        return false;

    if (!m_userInstallation) {
        qCWarning(lcFlatpakBackend) << "Repository file" << url.toLocalFile() << "won't be installed because no user installation is available";
        return false;
    }

    const QString name = url.fileName().remove(QLatin1String(".flatpakrepo"));

    if (url.isLocalFile()) {
        addLocalSource(name, url);
    } else {
        QNetworkAccessManager *manager = new QNetworkAccessManager(this);
        connect(manager, &QNetworkAccessManager::finished, this, [manager, name, this](QNetworkReply *reply) {
            if (reply->error() == QNetworkReply::NoError) {
                const QString uuid = QUuid::createUuid().toString();
                const QDir tempDir = QDir(QStandardPaths::writableLocation(QStandardPaths::TempLocation));
                const QString filePath = tempDir.absoluteFilePath(QStringLiteral("%1.flatpakrepo").arg(uuid));

                QByteArray data = reply->readAll();
                QFile file(filePath);
                if (file.open(QFile::WriteOnly)) {
                    file.write(data);
                    file.close();

                    addLocalSource(name, QUrl::fromLocalFile(filePath));
                }
            }

            manager->deleteLater();
        });
        manager->get(QNetworkRequest(url));
    }

    return true;
}

bool FlatpakBackend::removeSource(Liri::AppCenter::SoftwareSource *softwareSource)
{
    FlatpakSource *source = qobject_cast<FlatpakSource *>(softwareSource);
    if (source) {
        g_autoptr(GCancellable) cancellable = g_cancellable_new();
        g_autoptr(GError) error = nullptr;

        if (flatpak_installation_remove_remote(source->installation(), source->name().toUtf8().constData(), cancellable, &error)) {
            m_manager->sourcesModel()->removeSource(source);
            return true;
        } else {
            qCWarning(lcFlatpakBackend, "Failed to remove source \"%s\": %s",
                      source->name().toUtf8().constData(), error->message);
            return false;
        }
    }

    return false;
}

Liri::AppCenter::SoftwareResource *FlatpakBackend::installAppFromPath(const QString &filePath)
{
    if (filePath.endsWith(QLatin1String(".flatpakref")))
        return installFromFlatpakRef(filePath);
    return nullptr;
}

GPtrArray *FlatpakBackend::listUpdates(FlatpakInstallation *installation)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    GPtrArray *refs =
            flatpak_installation_list_installed_refs_for_update(installation, cancellable, &error);
    if (!refs) {
        const char *name = flatpak_installation_get_display_name(installation);
        qCWarning(lcFlatpakBackend, "Failed to list updates for installation \"%s\": %s",
                  name, error->message);
    }

    return refs;
}

FlatpakResource *FlatpakBackend::findResourceFromInstalledRef(FlatpakInstallation *installation, FlatpakInstalledRef *ref) const
{
    return m_resources.value(FlatpakResource::keyFromInstalledRef(installation, ref));
}

FlatpakResource *FlatpakBackend::findRuntimeResource(const QString &runtime)
{
    const auto runtimeInfo = runtime.splitRef(QLatin1Char('/'));
    if (runtimeInfo.count() != 3)
        return nullptr;

    for (auto it = m_resources.constBegin(); it != m_resources.constEnd(); ++it) {
        const auto &key = it.key();

        if (key.type == SoftwareResource::Runtime && key.desktopId == runtimeInfo[0] &&
                key.branch == runtimeInfo[2])
            return *it;
    }

    return nullptr;
}

bool FlatpakBackend::extractRepositories(FlatpakInstallation *installation)
{
    Q_ASSERT(installation != nullptr);
    Q_ASSERT(G_IS_OBJECT(installation));

    const char *installationName = flatpak_installation_get_display_name(installation);

    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;
    GPtrArray *remotes = flatpak_installation_list_remotes(installation, cancellable, &error);
    if (!remotes) {
        qCWarning(lcFlatpakBackend, "Failed to list repositories from installation \"%s\": %s",
                  installationName, error->message);
        return false;
    }

    for (uint i = 0; i < remotes->len; i++) {
        FlatpakRemote *remote = FLATPAK_REMOTE(g_ptr_array_index(remotes, i));

        if (flatpak_remote_get_noenumerate(remote))
            continue;

        const QString remoteName = QString::fromUtf8(flatpak_remote_get_name(remote));
        const QUrl url = QString::fromUtf8(flatpak_remote_get_url(remote));
        const QString title = QString::fromUtf8(flatpak_remote_get_title(remote));
        const int priority = flatpak_remote_get_prio(remote);
        const bool disabled = flatpak_remote_get_disabled(remote);

        FlatpakSource *source = new FlatpakSource(installation, remote);
        source->setBackend(this);
        source->setName(remoteName);
        source->setTitle(title);
        source->setEnabled(!disabled);
        source->setSection(QLatin1String("Flatpak"));
        source->setUrl(url);
        source->setPriority(priority);
        m_manager->sourcesModel()->addSource(source);
        Q_EMIT m_manager->sourceAdded(source);
    }

    return true;
}

bool FlatpakBackend::listAvailableApps(FlatpakInstallation *installation)
{
    Q_ASSERT(installation != nullptr);
    Q_ASSERT(G_IS_OBJECT(installation));

    g_autoptr(GError) error = nullptr;
    GPtrArray *remotes =
            flatpak_installation_list_remotes(installation, m_cancellable, &error);
    if (!remotes) {
        const char *name = flatpak_installation_get_display_name(installation);
        qCWarning(lcFlatpakBackend, "Failed to list available apps from installation %s: %s",
                  name, error->message);
        return false;
    }

    // Process remotes
    for (uint i = 0; i < remotes->len; i++) {
        FlatpakRemote *remote = FLATPAK_REMOTE(g_ptr_array_index(remotes, i));

        // Download AppStream metadata if it has never been fetched before or the cache is older than 6 hours
        g_autoptr(GFile) timestampFile = flatpak_remote_get_appstream_timestamp(remote, nullptr);
        QFileInfo timestampFileInfo(QString::fromUtf8(g_file_get_path(timestampFile)));
        if (!timestampFileInfo.exists() || timestampFileInfo.lastModified().toUTC().secsTo(QDateTime::currentDateTimeUtc()) > 21600)
            fetchAppStreamMetadata(installation, remote);
        else
            addAppsFromRemote(installation, remote);
    }

    return true;
}

bool FlatpakBackend::listInstalledApps(FlatpakInstallation *installation)
{
    Q_ASSERT(installation != nullptr);
    Q_ASSERT(G_IS_OBJECT(installation));

    g_autoptr(GError) error = nullptr;

    g_autoptr(GPtrArray) refs =
            flatpak_installation_list_installed_refs(installation, m_cancellable, &error);
    if (!refs) {
        const char *name = flatpak_installation_get_display_name(installation);
        qCWarning(lcFlatpakBackend, "Failed to list installed apps from installation %s: %s",
                  name, error->message);
        return false;
    }

    // Process refs
    for (uint i = 0; i < refs->len; i++) {
        FlatpakInstalledRef *ref = FLATPAK_INSTALLED_REF(g_ptr_array_index(refs, i));
        const QString name = QString::fromUtf8(flatpak_ref_get_name(FLATPAK_REF(ref)));

        // Ignore special apps
        if (name.endsWith(QLatin1String(".Debug")) || name.endsWith(QLatin1String(".Locale")) ||
                name.endsWith(QLatin1String(".BaseApp")) || name.endsWith(QLatin1String(".Docs")))
            continue;

        // Desktop entry
        const QDir exportsDir = FlatpakResource::installationDir(installation).absoluteFilePath(QLatin1String("exports/"));
        const QString desktopFilePath = exportsDir.absoluteFilePath(QLatin1String("share/applications/") +
                                                                    name + QLatin1String(".desktop"));
        if (!QFileInfo::exists(desktopFilePath))
            continue;

        // AppStream metadata
        AppStream::Metadata metadata;
        AppStream::Metadata::MetadataError error = metadata.parseFile(
                    desktopFilePath, AppStream::Metadata::FormatKindDesktopEntry);
        if (error != AppStream::Metadata::MetadataErrorNoError) {
            qCWarning(lcFlatpakBackend, "Failed to parse AppStream metadata from %s",
                      desktopFilePath.toUtf8().constData());
            continue;
        }

        for (auto component : metadata.components()) {
            // Create resource
            auto *resource = findResourceFromInstalledRef(installation, ref);
            if (!resource || resource->component().id() != component.id())
                resource = new FlatpakResource(component, installation);
            resource->updateFromRef(FLATPAK_REF(ref));
            resource->updateFromInstalledRef(ref);

            // Add the resource
            if (!m_resources.contains(resource->key())) {
                m_manager->resourcesModel()->addResource(resource);
                m_resources[resource->key()] = resource;
            }
        }
    }

    return true;
}

FlatpakResource *FlatpakBackend::installFromFlatpakRef(const QString &filePath)
{
    QSettings settings(filePath, QSettings::NativeFormat);
    settings.beginGroup(QLatin1String("Flatpak Ref"));

    const QString refUrl(settings.value(QLatin1String("Url")).toString());
    const QString iconUrl(settings.value(QLatin1String("Icon")).toString());
    const QString homepage(settings.value(QLatin1String("Homepage")).toString());
    const QUrl runtimeUrl(settings.value(QLatin1String("RuntimeRepo")).toString());

    // Read file
    g_autoptr(GError) error = nullptr;
    g_autoptr(FlatpakRemoteRef) remoteRef = nullptr;
    {
        QFile file(filePath);
        if (!file.open(QFile::ReadOnly | QFile::Text))
            return nullptr;

        QByteArray contents = file.readAll();
        g_autoptr(GBytes) bytes = g_bytes_new(contents.data(), contents.size());

        g_autoptr(FlatpakTransaction) transaction = flatpak_transaction_new_for_installation(m_userInstallation, m_cancellable, &error);
        if (!transaction) {
            qCWarning(lcFlatpakBackend, "Failed to install ref file \"%s\": %s",
                      qPrintable(filePath), error->message);
            return nullptr;
        }
        if (!flatpak_transaction_add_install_flatpakref(transaction, bytes, &error)) {
            qCWarning(lcFlatpakBackend, "Failed to install ref file \"%s\": %s",
                      qPrintable(filePath), error->message);
            return nullptr;
        }
    }

    FlatpakRef *ref = FLATPAK_REF(remoteRef);

    AppStream::Component component;
    component.setId(settings.value(QLatin1String("Name")).toString());
    component.setName(settings.value(QLatin1String("Title")).toString());
    component.setSummary(settings.value(QLatin1String("Comment")).toString());
    component.setDescription(settings.value(QLatin1String("Description")).toString());
    if (!homepage.isEmpty())
        component.addUrl(AppStream::Component::UrlKindHomepage, homepage);
    if (!iconUrl.isEmpty()) {
        AppStream::Icon icon;
        icon.setKind(AppStream::Icon::KindRemote);
        icon.setUrl(iconUrl);
        component.addIcon(icon);
    }

    FlatpakResource *resource = new FlatpakResource(component, m_userInstallation);
    resource->updateFromRef(ref);

    if (runtimeUrl.isEmpty()) {

    } else {

    }

    return resource;
}

bool FlatpakBackend::addLocalSource(const QString &name, const QUrl &url)
{
    // Read .flatpakrepo file
    QSettings settings(url.toLocalFile(), QSettings::NativeFormat);
    const QString title = settings.value(QLatin1String("Flatpak Repo/Title")).toString();
    const QUrl repoUrl = settings.value(QLatin1String("Flatpak Repo/Url")).toString();
    const QByteArray gpgKeyBase64 = settings.value(QLatin1String("Flatpak Repo/GPGKey")).toString().toLocal8Bit();

    if (name.isEmpty() || title.isEmpty() || repoUrl.isEmpty() || gpgKeyBase64.isEmpty()) {
        qCWarning(lcFlatpakBackend) << "Repository file" << url.toLocalFile() << "is incomplete";
        Q_EMIT m_manager->sourceAddFailed(name, tr("Repository file is incomplete"));
        return false;
    }

    // Convert QByteArray to GBytes
    QByteArray gpgKey = QByteArray::fromBase64(gpgKeyBase64);
    g_autoptr(GBytes) bytes = g_bytes_new(gpgKey.constData(), gpgKey.size());

    // Create remote
    FlatpakRemote *remote =
            flatpak_installation_get_remote_by_name(m_userInstallation, name.toUtf8().constData(),
                                                    nullptr, nullptr);
    if (!remote)
        remote = flatpak_remote_new(name.toUtf8().constData());
    flatpak_remote_set_title(remote, title.toUtf8().constData());
    flatpak_remote_set_url(remote, repoUrl.toString().toUtf8().constData());
    flatpak_remote_set_gpg_key(remote, bytes);
    flatpak_remote_set_gpg_verify(remote, true);
    flatpak_remote_set_noenumerate(remote, false);
    flatpak_remote_set_disabled(remote, false);

    // Install it
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_userInstallation, remote, cancellable, &error)) {
        // Refresh available apps
        fetchAppStreamMetadata(m_userInstallation, remote);

        // Add source to the model
        FlatpakSource *source = qobject_cast<FlatpakSource *>(m_manager->sourcesModel()->findSource(name));
        if (!source) {
            source = new FlatpakSource(m_userInstallation, remote);
            source->setBackend(this);
            source->setName(name);
            source->setTitle(title);
            source->setEnabled(true);
            source->setSection(QLatin1String("Flatpak"));
            source->setUrl(repoUrl);
            source->setPriority(flatpak_remote_get_prio(remote));
            m_manager->sourcesModel()->addSource(source);
            Q_EMIT m_manager->sourceAdded(source);
        }
    } else {
        qCWarning(lcFlatpakBackend,
                  "Unable to add the source \"%s\" to the user installation: %s",
                  name.toUtf8().constData(), error->message);
        Q_EMIT m_manager->sourceAddFailed(name, QString::fromUtf8(error->message));
        return false;
    }

    return true;
}

void FlatpakBackend::fetchAppStreamMetadata(FlatpakInstallation *installation, FlatpakRemote *remote)
{
    FlatpakAppStreamJob *job = new FlatpakAppStreamJob(installation, remote);
    connect(job, &FlatpakAppStreamJob::succeeded, this, &FlatpakBackend::addAppsFromRemote);
    connect(job, &FlatpakAppStreamJob::finished, job, &QObject::deleteLater);
    job->start();
}

bool FlatpakBackend::checkLocalUpdatesForInstallation(FlatpakInstallation *installation)
{
    g_autoptr(GError) error = nullptr;

    g_autoptr(GPtrArray) refs =
            flatpak_installation_list_installed_refs(installation, m_cancellable, &error);
    if (!refs) {
        const char *name = flatpak_installation_get_display_name(installation);
        qCWarning(lcFlatpakBackend, "Failed to list installed apps from installation %s: %s",
                  name, error->message);
        return false;
    }

    // Process refs
    for (uint i = 0; i < refs->len; i++) {
        FlatpakInstalledRef *ref = FLATPAK_INSTALLED_REF(g_ptr_array_index(refs, i));

        // Check the status again from the installed ref, we might have already
        // download a new version
        auto *resource = findResourceFromInstalledRef(installation, ref);
        if (resource) {
            resource->updateFromRef(FLATPAK_REF(ref));
            resource->updateFromInstalledRef(ref);
        }
    }

    return true;
}

void FlatpakBackend::checkUpdatesForInstallation(FlatpakInstallation *installation)
{
    auto watcher = new QFutureWatcher<GPtrArray *>(this);
    auto future = QtConcurrent::run(FlatpakBackend::listUpdates, installation);
    connect(watcher, &QFutureWatcher<GPtrArray *>::finished, this, [this, installation, watcher] {
        auto refs = watcher->result();
        updatesFetched(installation, refs);
        watcher->deleteLater();
    });
    watcher->setFuture(future);
}

void FlatpakBackend::updatesFetched(FlatpakInstallation *installation, GPtrArray *refs)
{
    uint numUpdates = refs->len;
    bool hasChanged = false;

    if (flatpak_installation_get_is_user(installation)) {
        hasChanged = m_userInstallationUpdates != numUpdates;
        m_userInstallationUpdates = numUpdates;
    } else {
        hasChanged = m_systemInstallationUpdates != numUpdates;
        m_systemInstallationUpdates = numUpdates;
    }

    for (uint i = 0; i < refs->len; i++) {
        FlatpakInstalledRef *ref = FLATPAK_INSTALLED_REF(g_ptr_array_index(refs, i));

        auto *resource = findResourceFromInstalledRef(installation, ref);
        if (resource) {
            resource->updateFromRef(FLATPAK_REF(ref));
            resource->updateFromInstalledRef(ref);

            // We might not have the latest commit ready to be installed:
            // force state given that Flatpak tells us this is upgradable
            resource->setState(SoftwareResource::UpgradableState);

            // Now we can update the download size
            resource->updateDownloadSize(findRuntimeResource(resource->runtime()), FLATPAK_REF(ref));
        }
    }

    if (hasChanged)
        Q_EMIT updatesAvailable(m_userInstallationUpdates + m_systemInstallationUpdates);
}

void FlatpakBackend::addAppsFromRemote(FlatpakInstallation *installation, FlatpakRemote *remote)
{
    // Locate AppStream metadata
    g_autoptr(GFile) file = flatpak_remote_get_appstream_dir(remote, nullptr);
    QDir appstreamDir(QString::fromUtf8(g_file_get_path(file)));
    QString appstreamFilePath = appstreamDir.absoluteFilePath(QLatin1String("appstream.xml.gz"));
    if (!QFileInfo::exists(appstreamFilePath))
        return;

    // AppStream metadata
    AppStream::Metadata metadata;
    metadata.setFormatStyle(AppStream::Metadata::FormatStyleCollection);
    AppStream::Metadata::MetadataError error = metadata.parseFile(
                appstreamFilePath, AppStream::Metadata::FormatKindXml);
    if (error != AppStream::Metadata::MetadataErrorNoError) {
        qCWarning(lcFlatpakBackend, "Failed to parse AppStream metadata from %s",
                  appstreamFilePath.toUtf8().constData());
        return;
    }

    // Create a resource for each component
    for (const auto &component : metadata.components()) {
        FlatpakResource *resource = new FlatpakResource(component, installation);
        resource->updateFromRemote(remote);

        // Add resource
        if (!m_resources.contains(resource->key())) {
            m_manager->resourcesModel()->addResource(resource);
            m_resources[resource->key()] = resource;
        }
    }

    g_object_unref(remote);
}
