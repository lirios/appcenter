// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
#include "internetcheck.h"

#include <sys/stat.h>

Q_LOGGING_CATEGORY(lcFlatpakBackend, "liri.appcenter.flatpak")

using namespace Liri::AppCenter;

static void changedCb(GFileMonitor *monitor, GFile *child, GFile *other_file,
                               GFileMonitorEvent event_type, gpointer data)
{
    Q_UNUSED(monitor);
    Q_UNUSED(child);
    Q_UNUSED(other_file);
    Q_UNUSED(event_type);

    auto *backend = static_cast<FlatpakBackend *>(data);
    if (!backend)
        return;

    backend->checkForUpdates();
}

FlatpakBackend::FlatpakBackend(Liri::AppCenter::SoftwareManager *manager,
                               QObject *parent)
    : Liri::AppCenter::Backend(manager, parent)
    , m_cancellable(g_cancellable_new())
{
    Q_ASSERT(manager);
    m_manager = manager;
}

FlatpakBackend::~FlatpakBackend()
{
    g_cancellable_cancel(m_cancellable);

    while (!m_installationMonitors.isEmpty())
        g_object_unref(m_installationMonitors.takeFirst());

    while (!m_installations.isEmpty())
        g_object_unref(m_installations.takeFirst());

    g_object_unref(m_cancellable);
}

void FlatpakBackend::initialize()
{
    /* Override the umask to 022 to make it possible to share files between
     * the liri-appcenter process and flatpak system helper process.
     * Ideally this should be set when needed in the flatpak plugin, but
     * umask is thread-unsafe so there is really no local way to fix this.
     *
     * See https://github.com/flatpak/flatpak/pull/2856/
     */
    umask(022);

    g_autoptr(GError) error = nullptr;

    GPtrArray *installations = flatpak_get_system_installations(m_cancellable, &error);
    if (!installations) {
        qCWarning(lcFlatpakBackend,
                  "Unable to list system installations: %s", error->message);
    }
    for (uint i = 0; installations && i < installations->len; i++) {
        auto *system = FLATPAK_INSTALLATION(g_ptr_array_index(installations, i));
        m_installations.append(system);

        auto *monitor = flatpak_installation_create_monitor(system, m_cancellable, &error);
        if (monitor) {
            g_signal_connect(monitor, "changed", G_CALLBACK(changedCb), this);
            m_installationMonitors.append(monitor);
        } else {
            qCWarning(lcFlatpakBackend,
                      "Unable to create a monitor for system-wide installation: %s",
                      error->message);
        }
    }

    auto *user = flatpak_installation_new_user(m_cancellable, &error);
    if (user) {
        m_userInstallation = user;
        m_installations.append(user);

        auto *monitor = flatpak_installation_create_monitor(user, m_cancellable, &error);
        if (monitor) {
            g_signal_connect(monitor, "changed", G_CALLBACK(changedCb), this);
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
    for (auto *installation : qAsConst(m_installations))
        extractRepositories(installation);
}

void FlatpakBackend::listAvailableApps()
{
    for (auto *installation : qAsConst(m_installations))
        listAvailableApps(installation);
}

void FlatpakBackend::listInstalledApps()
{
    for (auto *installation : qAsConst(m_installations))
        listInstalledApps(installation);
}

void FlatpakBackend::checkForUpdates()
{
    for (auto installation : qAsConst(m_installations)) {
        checkLocalUpdatesForInstallation(installation);
        checkUpdatesForInstallation(installation);
    }
}

QVector<Liri::AppCenter::SoftwareResource *> FlatpakBackend::updates() const
{
    QVector<SoftwareResource *> list;

    const auto keys = m_resources.keys();
    for (const auto &id : keys) {
        const auto resources = m_resources.values(id);
        for (auto *resource : resources) {
            if (resource->type() == SoftwareResource::App &&
                    resource->state() == SoftwareResource::UpgradableState)
                list.append(resource);
        }
    }

    return list;
}

bool FlatpakBackend::addSource(const QString &source)
{
    const QUrl url = QUrl::fromUserInput(source);

    if (!url.fileName().endsWith(QLatin1String(".flatpakrepo")))
        return false;

    if (!m_userInstallation) {
        qCWarning(lcFlatpakBackend,
                  "Repository file \"%s\" won't be installed because no user "
                  "installation is available",
                  qPrintable(url.toLocalFile()));
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
        g_autoptr(GError) error = nullptr;

        if (flatpak_installation_remove_remote(source->installation(),
                                               source->name().toUtf8().constData(),
                                               m_cancellable, &error)) {
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

QList<AppStream::Component> FlatpakBackend::componentsFromInstalledRef(FlatpakInstallation *installation,
                                                                       FlatpakInstalledRef *ref)
{
    const QString name = QString::fromUtf8(flatpak_ref_get_name(FLATPAK_REF(ref)));

    // Ignore special apps
    if (name.endsWith(QLatin1String(".Debug")) || name.endsWith(QLatin1String(".Locale")) ||
            name.endsWith(QLatin1String(".BaseApp")) || name.endsWith(QLatin1String(".Docs")))
        return QList<AppStream::Component>();

    // Desktop entry
    const QDir exportsDir = FlatpakResource::installationDir(installation).absoluteFilePath(QLatin1String("exports/"));
    const QString desktopFilePath = exportsDir.absoluteFilePath(QLatin1String("share/applications/") +
                                                                name + QLatin1String(".desktop"));
    if (!QFileInfo::exists(desktopFilePath))
        return QList<AppStream::Component>();

    // AppStream metadata
    AppStream::Metadata metadata;
    AppStream::Metadata::MetadataError error = metadata.parseFile(
                desktopFilePath, AppStream::Metadata::FormatKindDesktopEntry);
    if (error != AppStream::Metadata::MetadataErrorNoError) {
        qCWarning(lcFlatpakBackend, "Failed to parse AppStream metadata from %s",
                  desktopFilePath.toUtf8().constData());
        return QList<AppStream::Component>();
    }

    return metadata.components();
}

FlatpakResource *FlatpakBackend::findRuntimeResource(const QString &runtime)
{
    const auto runtimeInfo = runtime.splitRef(QLatin1Char('/'));
    if (runtimeInfo.count() != 3)
        return nullptr;

    const auto keys = m_resources.keys();
    for (const auto &id : keys) {
        const auto resources = m_resources.values(id);
        for (auto *resource : resources) {
            if (resource->type() == SoftwareResource::Runtime &&
                    resource->appId() == runtimeInfo[0] &&
                    resource->branch() == runtimeInfo[2])
                return resource;
        }
    }

    return nullptr;
}

bool FlatpakBackend::extractRepositories(FlatpakInstallation *installation)
{
    Q_ASSERT(installation != nullptr);
    Q_ASSERT(G_IS_OBJECT(installation));

    const char *installationName = flatpak_installation_get_display_name(installation);

    g_autoptr(GError) error = nullptr;
    GPtrArray *remotes = flatpak_installation_list_remotes(installation, m_cancellable, &error);
    if (!remotes) {
        qCWarning(lcFlatpakBackend, "Failed to list repositories from installation \"%s\": %s",
                  installationName, error->message);
        return false;
    }

    for (uint i = 0; i < remotes->len; i++) {
        FlatpakRemote *remote = FLATPAK_REMOTE(g_ptr_array_index(remotes, i));

        if (flatpak_remote_get_noenumerate(remote))
            continue;

        FlatpakSource *source = new FlatpakSource(this, installation, remote, this);
        m_sources.append(source);
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

    // Internet check
    auto *check = new InternetCheck(this);
    bool hasInternet = check->hasInternet();
    check->deleteLater();

    // Process remotes
    for (uint i = 0; i < remotes->len; i++) {
        FlatpakRemote *remote = FLATPAK_REMOTE(g_ptr_array_index(remotes, i));

        if (hasInternet) {
            // Download AppStream metadata if it has never been fetched before or the cache is older than 6 hours
            g_autoptr(GFile) timestampFile = flatpak_remote_get_appstream_timestamp(remote, nullptr);
            QFileInfo timestampFileInfo(QString::fromUtf8(g_file_get_path(timestampFile)));
            if (!timestampFileInfo.exists() || timestampFileInfo.lastModified().toUTC().secsTo(QDateTime::currentDateTimeUtc()) > 21600)
                fetchAppStreamMetadata(installation, remote);
            else
                addAppsFromRemote(installation, remote);
        } else {
            // Always use existing metadata if Internet doesn't work
            addAppsFromRemote(installation, remote);
        }
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
        auto origin = QString::fromUtf8(flatpak_installed_ref_get_origin(ref));
        const auto components = componentsFromInstalledRef(installation, ref);

        for (const auto &component : components) {
            const auto resources = m_resources.values(component.id());
            for (auto *resource : resources) {
                // Update all resources from the same installation
                if (resource->flatpakSource()->installation() == installation &&
                        resource->source()->name() == origin) {
                    resource->updateFromRef(FLATPAK_REF(ref));
                    resource->updateFromInstalledRef(ref);
                }
            }
        }
    }

    return true;
}

FlatpakResource *FlatpakBackend::installFromFlatpakRef(const QString &filePath)
{
    QSettings settings(filePath, QSettings::NativeFormat);
    settings.beginGroup(QLatin1String("Flatpak Ref"));

    const QString iconUrl(settings.value(QLatin1String("Icon")).toString());
    const QString homepage(settings.value(QLatin1String("Homepage")).toString());
    const QUrl runtimeUrl(settings.value(QLatin1String("RuntimeRepo")).toString());

    // Read file
    g_autoptr(GError) error = nullptr;
    //g_autoptr(FlatpakRemoteRef) remoteRef = nullptr;
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

    //FlatpakRef *ref = FLATPAK_REF(remoteRef);

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

    // FIXME
    //FlatpakResource *resource = new FlatpakResource(m_manager, component, m_userInstallation, this);
    //resource->updateFromRef(ref);

    if (runtimeUrl.isEmpty()) {

    } else {

    }

    return nullptr;
}

bool FlatpakBackend::addLocalSource(const QString &name, const QUrl &url)
{
    // Read .flatpakrepo file
    QSettings settings(url.toLocalFile(), QSettings::NativeFormat);
    const QString title = settings.value(QLatin1String("Flatpak Repo/Title")).toString();
    const QUrl repoUrl = settings.value(QLatin1String("Flatpak Repo/Url")).toString();
    const QByteArray gpgKeyBase64 = settings.value(QLatin1String("Flatpak Repo/GPGKey")).toString().toLocal8Bit();

    if (name.isEmpty() || title.isEmpty() || repoUrl.isEmpty() || gpgKeyBase64.isEmpty()) {
        qCWarning(lcFlatpakBackend, "Repository file \"%s\" is incomplete",
                  qPrintable(url.toLocalFile()));
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
    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_userInstallation, remote, m_cancellable, &error)) {
        // Refresh available apps
        fetchAppStreamMetadata(m_userInstallation, remote);

        // Add source to the model
        FlatpakSource *source = qobject_cast<FlatpakSource *>(m_manager->sourcesModel()->findSource(name));
        if (!source) {
            source = new FlatpakSource(this, m_userInstallation, remote, this);
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
    connect(job, &FlatpakAppStreamJob::failed, this, [this, installation, remote](const QString &errorMessage) {
        // Reuse a previously downloaded copy, if any
        qCWarning(lcFlatpakBackend, "Failed to update AppStream metadata for \"%s\": %s",
                  flatpak_remote_get_name(remote),
                  qPrintable(errorMessage));
        addAppsFromRemote(installation, remote);
    });
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
        auto origin = QString::fromUtf8(flatpak_installed_ref_get_origin(ref));
        const auto components = componentsFromInstalledRef(installation, ref);

        // Check the status again from the installed ref, we might have already
        // download a new version
        for (const auto &component : components) {
            const auto resources = m_resources.values(component.id());
            for (auto *resource : resources) {
                // Update all resources from the same installation and remote
                if (resource->flatpakSource()->installation() == installation &&
                        resource->source()->name() == origin) {
                    resource->updateFromRef(FLATPAK_REF(ref));
                    resource->updateFromInstalledRef(ref);
                }
            }
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
    uint numUpdates = 0;

    for (uint i = 0; i < refs->len; i++) {
        FlatpakInstalledRef *ref = FLATPAK_INSTALLED_REF(g_ptr_array_index(refs, i));
        auto origin = QString::fromUtf8(flatpak_installed_ref_get_origin(ref));
        const auto components = componentsFromInstalledRef(installation, ref);

        for (const auto &component : components) {
            const auto resources = m_resources.values(component.id());
            for (auto *resource : resources) {
                // Update all resources from the same installation and remote
                if (resource->flatpakSource()->installation() == installation &&
                        resource->source()->name() == origin) {
                    resource->updateFromRef(FLATPAK_REF(ref));
                    resource->updateFromInstalledRef(ref);

                    // We might not have the latest commit ready to be installed:
                    // force state given that Flatpak tells us this is upgradable
                    resource->setState(SoftwareResource::UpgradableState);

                    // Now we can update the download size
                    resource->updateDownloadSize(findRuntimeResource(resource->runtime()), FLATPAK_REF(ref));

                    numUpdates++;
                }
            }
        }
    }

    Q_EMIT updatesAvailable(numUpdates);
}

void FlatpakBackend::addAppsFromRemote(FlatpakInstallation *installation, FlatpakRemote *remote)
{
    // Remote name
    auto name = QString::fromUtf8(flatpak_remote_get_name(remote));

    // Locate AppStream metadata
    g_autoptr(GFile) file = flatpak_remote_get_appstream_dir(remote, nullptr);
    QDir appstreamDir(QString::fromUtf8(g_file_get_path(file)));
    QString appstreamFilePath = appstreamDir.absoluteFilePath(QLatin1String("appstream.xml.gz"));
    if (!QFileInfo::exists(appstreamFilePath))
        return;

    // AppStream metadata
    AppStream::Metadata metadata;
    metadata.setFormatStyle(AppStream::Metadata::FormatStyleCatalog);
    AppStream::Metadata::MetadataError error = metadata.parseFile(
                appstreamFilePath, AppStream::Metadata::FormatKindXml);
    if (error != AppStream::Metadata::MetadataErrorNoError) {
        qCWarning(lcFlatpakBackend, "Failed to parse AppStream metadata from %s",
                  appstreamFilePath.toUtf8().constData());
        return;
    }

    // Create a resource for each component
    const auto components = metadata.components();
    for (const auto &component : components) {
        FlatpakResource *resource = nullptr;

        // Find an already existing resource
        if (m_resources.contains(component.id())) {
            const auto resources = m_resources.values(component.id());
            for (auto *r : resources) {
                if (r->flatpakSource()->installation() == installation &&
                        r->source()->name() == name) {
                    resource = r;
                    break;
                }
            }
        }

        // Resource not found: create a new one
        if (!resource) {
            // Find source
            FlatpakSource *source = nullptr;
            for (auto *curSource : qAsConst(m_sources)) {
                if (curSource->installation() == installation && curSource->name() == name) {
                    source = curSource;
                    break;
                }
            }
            if (!source) {
                qCWarning(lcFlatpakBackend, "Cannot find source for remote \"%s\"",
                          qPrintable(name));
                return;
            }

            // Create resource
            resource = new FlatpakResource(m_manager, source, component, this);
            m_resources.insert(component.id(), resource);
            m_manager->addResource(component.id(), resource);
        }
    }

    m_installationsListed.append(installation);
}
