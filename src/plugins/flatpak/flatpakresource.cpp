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
#include <QFutureWatcher>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QSettings>
#include <QStandardPaths>

#include <AppStreamQt/icon.h>
#include <AppStreamQt/image.h>
#include <AppStreamQt/screenshot.h>

#include "flatpakplugin.h"
#include "flatpakresource.h"
#include "flatpaktransaction.h"

static quint64 fetchRemoteSize(FlatpakResource *app, FlatpakRef *ref)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    guint64 downloadSize = 0, installedSize = 0;
    if (!flatpak_installation_fetch_remote_size_sync(
                app->installation(), qPrintable(app->origin()),
                ref, &downloadSize, &installedSize, cancellable, &error)) {
        qCWarning(lcFlatpakBackend, "Failed to fetch remote size for %s: %s",
                  qPrintable(app->name()), error->message);
        return 0;
    }

    return downloadSize;
}

FlatpakResource::FlatpakResource(const AppStream::Component &component,
                                 FlatpakInstallation *installation,
                                 QObject *parent)
    : Liri::AppCenter::SoftwareResource(parent)
    , m_appdata(component)
{
    updateComponent(component);
    setFlatpakType(FLATPAK_REF_KIND_APP);

    m_key.installation = installation;
    m_key.desktopId = component.id();
}

FlatpakResource::Key FlatpakResource::keyFromInstalledRef(FlatpakInstallation *installation, FlatpakInstalledRef *ref)
{
    Liri::AppCenter::SoftwareResource::Type type = Liri::AppCenter::SoftwareResource::App;
    if (flatpak_ref_get_kind(FLATPAK_REF(ref)) == FLATPAK_REF_KIND_RUNTIME)
        type = Liri::AppCenter::SoftwareResource::Runtime;

    const auto desktopId = QLatin1String(flatpak_ref_get_name(FLATPAK_REF(ref)));
    const auto origin = QString::fromUtf8(flatpak_installed_ref_get_origin(FLATPAK_INSTALLED_REF(ref)));
    const auto branch = QString::fromUtf8(flatpak_ref_get_branch(FLATPAK_REF(ref)));
    const auto architecture = QString::fromUtf8(flatpak_ref_get_arch(FLATPAK_REF(ref)));

    return { type, installation, desktopId, origin, branch, architecture };
}

FlatpakResource::Key FlatpakResource::key() const
{
    return m_key;
}

AppStream::Component FlatpakResource::component() const
{
    return m_appdata;
}

FlatpakInstallation *FlatpakResource::installation() const
{
    return m_key.installation;
}

FlatpakRefKind FlatpakResource::kind() const
{
    return m_kind;
}

Liri::AppCenter::SoftwareResource::Type FlatpakResource::type() const
{
    return m_key.type;
}

Liri::AppCenter::SoftwareResource::State FlatpakResource::state() const
{
    return m_state;
}

QString FlatpakResource::name() const
{
    QString name = m_appdata.name();
    if (name.isEmpty())
        name = m_appdata.id();
    if (name.startsWith(QLatin1String("(Nightly) ")))
        return name.mid(10);
    return name;
}

QString FlatpakResource::summary() const
{
    return m_appdata.summary();
}

QString FlatpakResource::description() const
{
    return m_appdata.description();
}

QString FlatpakResource::desktopId() const
{
    return m_key.desktopId;
}

QString FlatpakResource::packageName() const
{
    return m_packageName;
}

QString FlatpakResource::architecture() const
{
    return m_key.architecture;
}

QString FlatpakResource::license() const
{
    QString license = m_appdata.projectLicense();
    if (license.startsWith(QLatin1String("LicenseRef-proprietary="))) {
        license = license.replace(QLatin1String("LicenseRef-proprietary="), QString());
        return QStringLiteral("<a href=\"%1\">Proprietary</a>").arg(license);
    }
    return license;
}

QString FlatpakResource::origin() const
{
    return m_key.origin;
}

QStringList FlatpakResource::categories() const
{
    return m_appdata.categories();
}

QUrl FlatpakResource::homepageUrl() const
{
    return m_appdata.url(AppStream::Component::UrlKindHomepage);
}

QUrl FlatpakResource::bugtrackerUrl() const
{
    return m_appdata.url(AppStream::Component::UrlKindBugtracker);
}

QUrl FlatpakResource::faqUrl() const
{
    return m_appdata.url(AppStream::Component::UrlKindFaq);
}

QUrl FlatpakResource::helpUrl() const
{
    return m_appdata.url(AppStream::Component::UrlKindHelp);
}

QUrl FlatpakResource::donationUrl() const
{
    return m_appdata.url(AppStream::Component::UrlKindDonation);
}

QUrl FlatpakResource::translateUrl() const
{
    return m_appdata.url(AppStream::Component::UrlTranslate);
}

QString FlatpakResource::installedVersion() const
{
    return branch();
}

QString FlatpakResource::availableVersion() const
{
    return branch();
}

bool FlatpakResource::updatesAvailable() const
{
    return m_commit != m_latestCommit;
}

quint64 FlatpakResource::downloadSize() const
{
    return m_downloadSize;
}

quint64 FlatpakResource::installedSize() const
{
    return m_installedSize;
}

QString FlatpakResource::changeLog() const
{
    return m_changeLog;
}

QStringList FlatpakResource::iconNames() const
{
    return m_stockIcons;
}

QVector<QUrl> FlatpakResource::iconUrls() const
{
    return m_icons;
}

QVector<Liri::AppCenter::Image> FlatpakResource::thumbnails() const
{
    return m_thumbnails;
}

QVector<Liri::AppCenter::Image> FlatpakResource::screenshots() const
{
    return m_screenshots;
}

bool FlatpakResource::isLocalized() const
{
    return m_appdata.language(m_appdata.activeLocale()) > 0;
}

bool FlatpakResource::launch() const
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    if (!flatpak_installation_launch(m_key.installation,
                                     packageName().toUtf8().constData(),
                                     architecture().toUtf8().constData(),
                                     branch().toUtf8().constData(),
                                     nullptr, cancellable, &error)) {
        qCWarning(lcFlatpakBackend, "Failed to launch \"%s\": %s",
                  m_appdata.name().toUtf8().constData(),
                  error->message);
        return false;
    }

    return true;
}

Liri::AppCenter::Transaction *FlatpakResource::install()
{
    LiriFlatpakTransaction *transaction = new LiriFlatpakTransaction(
                Liri::AppCenter::Transaction::Install,
                tr("Install %1").arg(packageName()),
                tr("Installing \"%1\" from %2...").arg(name()).arg(origin()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::installed);
    return transaction;
}

Liri::AppCenter::Transaction *FlatpakResource::uninstall()
{
    LiriFlatpakTransaction *transaction = new LiriFlatpakTransaction(
                Liri::AppCenter::Transaction::Uninstall,
                tr("Uninstall %1 %2").arg(packageName()).arg(installedVersion()),
                tr("Uninstalling \"%1\" %2...").arg(name()).arg(installedVersion()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::uninstalled);
    return transaction;
}

Liri::AppCenter::Transaction *FlatpakResource::update()
{
    LiriFlatpakTransaction *transaction = new LiriFlatpakTransaction(
                Liri::AppCenter::Transaction::Update,
                tr("Update %1 %2").arg(packageName()).arg(installedVersion()),
                tr("Updating \"%1\" %2...").arg(name()).arg(installedVersion()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::updated);
    return transaction;
}

QDir FlatpakResource::installationDir() const
{
    return FlatpakResource::installationDir(m_key.installation);
}

QDir FlatpakResource::exportsDir() const
{
    return installationDir().absoluteFilePath(QLatin1String("exports"));
}

QString FlatpakResource::runtime() const
{
    return m_runtime;
}

QString FlatpakResource::branch() const
{
    return m_key.branch;
}

QString FlatpakResource::commit() const
{
    return m_commit;
}

QString FlatpakResource::ref() const
{
    QString refKind;

    switch (kind()) {
    case FLATPAK_REF_KIND_APP:
        refKind = QStringLiteral("app");
        break;
    case FLATPAK_REF_KIND_RUNTIME:
        refKind = QStringLiteral("runtime");
        break;
    }

    return QStringLiteral("%1/%2/%3/%4").arg(
                refKind, packageName(), architecture(), branch());
}

void FlatpakResource::updateFromResource(FlatpakResource *resource)
{
    setState(resource->state());
}

void FlatpakResource::updateFromRemote(FlatpakRemote *remote)
{
    Q_ASSERT(remote);

    m_key.origin = QString::fromUtf8(flatpak_remote_get_name(remote));

    g_autoptr(GFile) appStreamDir = flatpak_remote_get_appstream_dir(remote, nullptr);
    if (appStreamDir)
        m_iconsPath = QDir(QString::fromUtf8(g_file_get_path(appStreamDir))).absoluteFilePath(QLatin1String("icons"));
}

void FlatpakResource::updateFromRef(FlatpakRef *ref)
{
    if (ref) {
        m_packageName = QString::fromLocal8Bit(flatpak_ref_get_name(ref));
        m_key.architecture = QString::fromLocal8Bit(flatpak_ref_get_arch(ref));

        m_key.branch = QString::fromLocal8Bit(flatpak_ref_get_branch(ref));
        if (m_key.branch.isEmpty())
            m_key.branch = QLatin1String("master");
        m_commit = QString::fromLocal8Bit(flatpak_ref_get_commit(ref));
        m_latestCommit = m_commit;

        setFlatpakType(flatpak_ref_get_kind(ref));
    } else {
        m_packageName = m_appdata.id().remove(QLatin1String(".desktop"));
        m_key.architecture = QString();
        m_key.branch = QLatin1String("master");
        m_commit = QString();
        m_latestCommit = QString();

        setFlatpakType(FLATPAK_REF_KIND_APP);
    }
}

void FlatpakResource::updateFromInstalledRef(FlatpakInstalledRef *ref)
{
    if (ref) {
        m_key.origin = QString::fromLocal8Bit(flatpak_installed_ref_get_origin(ref));
        m_latestCommit = QString::fromLocal8Bit(flatpak_installed_ref_get_latest_commit(ref));
        m_installedSize = flatpak_installed_ref_get_installed_size(ref);
        setState(updatesAvailable() ? SoftwareResource::UpgradableState : SoftwareResource::InstalledState);
    } else {
        m_latestCommit = QString();
        m_installedSize = 0;
        setState(SoftwareResource::NotInstalledState);
    }

    Q_EMIT versionChanged();
    Q_EMIT installedVersionChanged();
    Q_EMIT availableVersionChanged();
    Q_EMIT updatesAvailableChanged();
}

void FlatpakResource::updateDownloadSize(FlatpakResource *runtimeResource, FlatpakRef *ref)
{
    if (m_key.type != SoftwareResource::App)
        return;

    quint64 size = 0;

    // If the runtime is not installed we count its size too
    if (runtimeResource && runtimeResource->state() != SoftwareResource::InstalledState)
        size = runtimeResource->downloadSize();

    // Now calculate this app download size
    if (state() != SoftwareResource::InstalledState) {
        auto *watcher = new QFutureWatcher<quint64>(this);
        auto future = QtConcurrent::run(fetchRemoteSize, this, ref);
        connect(watcher, &QFutureWatcher<quint64>::finished, this, [this, size, watcher] {
            m_downloadSize = watcher->result() + size;
            watcher->deleteLater();
        });
        watcher->setFuture(future);
    }
}

bool FlatpakResource::updateFromMetadata()
{
    const QString filePath =
            installationDir().absoluteFilePath(QStringLiteral("/app/%1/%2/%3/active/metadata")
                                               .arg(packageName())
                                               .arg(architecture())
                                               .arg(branch()));
    if (QFile::exists(filePath)) {
        // Parse metadata in INI format
        QSettings settings(filePath, QSettings::NativeFormat);
        m_runtime = settings.value(QLatin1String("Application/runtime")).toString();
        return true;
    } else {

    }

    return false;
}

QDir FlatpakResource::installationDir(FlatpakInstallation *installation)
{
    g_autoptr(GFile) path = flatpak_installation_get_path(installation);
    return QDir(QString::fromUtf8(g_file_get_path(path)));
}

void FlatpakResource::setFlatpakType(FlatpakRefKind kind)
{
    m_kind = kind;

    if (kind == FLATPAK_REF_KIND_APP) {
        switch (m_appdata.kind()) {
        case AppStream::Component::KindAddon:
            m_key.type = Liri::AppCenter::SoftwareResource::Addon;
            break;
        case AppStream::Component::KindCodec:
            m_key.type = Liri::AppCenter::SoftwareResource::Codec;
            break;
        case AppStream::Component::KindDriver:
            m_key.type = Liri::AppCenter::SoftwareResource::Driver;
            break;
        case AppStream::Component::KindFirmware:
            m_key.type = Liri::AppCenter::SoftwareResource::Firmware;
            break;
        case AppStream::Component::KindFont:
            m_key.type = Liri::AppCenter::SoftwareResource::Font;
            break;
        case AppStream::Component::KindGeneric:
            m_key.type = Liri::AppCenter::SoftwareResource::Generic;
            break;
        case AppStream::Component::KindInputmethod:
            m_key.type = Liri::AppCenter::SoftwareResource::InputMethod;
            break;
        case AppStream::Component::KindLocalization:
            m_key.type = Liri::AppCenter::SoftwareResource::Localization;
            break;
        default:
            m_key.type = Liri::AppCenter::SoftwareResource::App;
            break;
        }
    } else {
        if (m_packageName.endsWith(QLatin1String(".Locale")))
            m_key.type = Liri::AppCenter::SoftwareResource::Localization;
        else if (m_packageName.endsWith(QLatin1String(".Debug")) ||
                 m_packageName.endsWith(QLatin1String(".Sources")))
            m_key.type = Liri::AppCenter::SoftwareResource::Generic;
        else if (m_packageName.startsWith(QLatin1String("org.freedesktop.Platform.Icontheme.")) ||
                 m_packageName.startsWith(QLatin1String("org.gtk.Gtk3theme.")))
            m_key.type = Liri::AppCenter::SoftwareResource::Theme;
        else
            m_key.type = Liri::AppCenter::SoftwareResource::Runtime;
    }
}

void FlatpakResource::setState(Liri::AppCenter::SoftwareResource::State state)
{
    m_state = state;
    Q_EMIT stateChanged();
}

void FlatpakResource::updateComponent(const AppStream::Component &component)
{
    m_appdata = component;

    // Desktop id
    m_key.desktopId = m_appdata.id();
    if (!m_key.desktopId.endsWith(QLatin1String(".desktop")))
        m_key.desktopId += QLatin1String(".desktop");

    // Origin
    m_key.origin = m_appdata.origin();

    // Change log
    m_changeLog = m_appdata.description();
    m_changeLog.replace(QLatin1Char('\n'), QLatin1String("<br>"));

    // Icons
    for (const auto &icon : m_appdata.icons()) {
        if (icon.kind() == AppStream::Icon::KindStock) {
            m_stockIcons.append(icon.name());
        } else if (icon.kind() == AppStream::Icon::KindRemote) {
            // Download remote icon if it wasn't cached before
            const QDir cacheDir = QDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
            const QString filePath = cacheDir.absoluteFilePath(QStringLiteral("icons/%2").arg(icon.url().fileName()));
            if (QFileInfo::exists(filePath)) {
                m_icons.append(QUrl::fromLocalFile(filePath));
            } else {
                cacheDir.mkdir(QLatin1String("icons"));

                QNetworkAccessManager *manager = new QNetworkAccessManager(this);
                connect(manager, &QNetworkAccessManager::finished, this, [filePath, this, manager](QNetworkReply *reply) {
                    if (reply->error() == QNetworkReply::NoError) {
                        QByteArray data = reply->readAll();
                        QFile file(filePath);
                        if (file.open(QFile::WriteOnly)) {
                            file.write(data);
                            file.close();
                            m_icons.append(QUrl::fromLocalFile(filePath));
                        }
                    }

                    manager->deleteLater();
                });
                manager->get(QNetworkRequest(icon.url()));
            }
        } else if (icon.kind() == AppStream::Icon::KindLocal) {
            if (QFileInfo::exists(icon.url().toLocalFile()))
                m_icons.append(icon.url());
        } else if (icon.kind() == AppStream::Icon::KindCached && !m_iconsPath.isEmpty()) {
            const QString path = QStringLiteral("%1/%2x%3/%4")
                    .arg(m_iconsPath)
                    .arg(icon.width())
                    .arg(icon.height())
                    .arg(icon.url().toLocalFile());
            if (QFileInfo::exists(path))
                m_icons.append(QUrl::fromLocalFile(path));
        }
    }

    // Screenshots and thumbnails
    for (const auto &screenshot : m_appdata.screenshots()) {
        bool thumbnailDone = false;
        bool screenshotDone = false;

        for (const auto &image : screenshot.images()) {
            if (image.kind() == AppStream::Image::KindThumbnail && !thumbnailDone) {
                Liri::AppCenter::Image thumbnailImage(image.url(), image.size());
                m_thumbnails.append(thumbnailImage);
                thumbnailDone = true;
            } else if (image.kind() == AppStream::Image::KindSource && !screenshotDone) {
                Liri::AppCenter::Image screenshotImage(image.url(), image.size());
                m_screenshots.append(screenshotImage);
                screenshotDone = true;
            }
        }
    }

    // Package information
    for (const auto &bundle : m_appdata.bundles()) {
        if (bundle.kind() != AppStream::Bundle::KindFlatpak)
            continue;

        g_autoptr(GError) error = nullptr;
        FlatpakRef *ref = flatpak_ref_parse(bundle.id().toUtf8().constData(), &error);
        if (ref) {
            updateFromRef(ref);
        } else {
            qCWarning(lcFlatpakBackend, "Unable to parse Flatpak identifier \"%s\" from AppStream: %s",
                      bundle.id().toUtf8().constData(), error->message);
        }
    }
}
