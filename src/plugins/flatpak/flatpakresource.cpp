// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QtConcurrentRun>
#include <QFutureWatcher>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVector>
#include <QStandardPaths>
#include <QXmlStreamReader>

#include <AppStreamQt/icon.h>
#include <AppStreamQt/image.h>
#include <AppStreamQt/release.h>
#include <AppStreamQt/screenshot.h>

#include "flatpakplugin.h"
#include "flatpakresource.h"
#include "flatpaksource.h"
#include "flatpaktransaction.h"
#include "flatpakutils.h"

static quint64 fetchRemoteSize(FlatpakResource *app, FlatpakRef *ref)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    guint64 downloadSize = 0, installedSize = 0;
    if (!flatpak_installation_fetch_remote_size_sync(
                app->flatpakSource()->installation(),
                qPrintable(app->flatpakSource()->name()),
                ref, &downloadSize, &installedSize, cancellable, &error)) {
        qCWarning(lcFlatpakBackend, "Failed to fetch remote size for %s: %s",
                  qPrintable(app->name()), error->message);
        return 0;
    }

    return downloadSize;
}

FlatpakResource::FlatpakResource(Liri::AppCenter::SoftwareManager *manager,
                                 FlatpakSource *source,
                                 const AppStream::Component &component,
                                 QObject *parent)
    : Liri::AppCenter::SoftwareResource(manager, source, parent)
    , m_appdata(component)
    , m_source(source)
{
    // Default branch from remote
    if (m_branch.isEmpty())
        m_branch = QString::fromUtf8(flatpak_remote_get_default_branch(m_source->remote()));

    // Icons path from remote
    g_autoptr(GFile) appStreamDir = flatpak_remote_get_appstream_dir(m_source->remote(), nullptr);
    if (appStreamDir)
        m_iconsPath = QDir(QString::fromUtf8(g_file_get_path(appStreamDir))).absoluteFilePath(QLatin1String("icons"));

    // Update information
    updateComponent();
    setFlatpakType(FLATPAK_REF_KIND_APP);

    // We always get this
    addKudo(SoftwareResource::SandboxedKudo);
}

FlatpakSource *FlatpakResource::flatpakSource() const
{
    return m_source;
}

AppStream::Component FlatpakResource::component() const
{
    return m_appdata;
}

FlatpakRefKind FlatpakResource::kind() const
{
    return m_kind;
}

Liri::AppCenter::SoftwareResource::Type FlatpakResource::type() const
{
    return m_type;
}

Liri::AppCenter::SoftwareResource::State FlatpakResource::state() const
{
    return m_state;
}

QString FlatpakResource::appId() const
{
    return m_appdata.id();
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

QString FlatpakResource::packageName() const
{
    return m_packageName;
}

QString FlatpakResource::architecture() const
{
    return m_architecture;
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

    if (!flatpak_installation_launch(m_source->installation(),
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
                tr("Installing \"%1\" from %2...").arg(name(), m_source->name()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::installed);
    return transaction;
}

Liri::AppCenter::Transaction *FlatpakResource::uninstall()
{
    LiriFlatpakTransaction *transaction = new LiriFlatpakTransaction(
                Liri::AppCenter::Transaction::Uninstall,
                tr("Uninstall %1 %2").arg(packageName(), installedVersion()),
                tr("Uninstalling \"%1\" %2...").arg(name(), installedVersion()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::uninstalled);
    return transaction;
}

Liri::AppCenter::Transaction *FlatpakResource::update()
{
    LiriFlatpakTransaction *transaction = new LiriFlatpakTransaction(
                Liri::AppCenter::Transaction::Update,
                tr("Update %1 %2").arg(packageName(), installedVersion()),
                tr("Updating \"%1\" %2...").arg(name(), installedVersion()),
                this, true, this);
    connect(transaction, &LiriFlatpakTransaction::succeeded,
            this, &FlatpakResource::updated);
    return transaction;
}

QDir FlatpakResource::installationDir() const
{
    return FlatpakResource::installationDir(m_source->installation());
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
    return m_branch;
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

void FlatpakResource::updateFromRef(FlatpakRef *ref)
{
    if (ref) {
        m_packageName = QString::fromUtf8(flatpak_ref_get_name(ref));
        m_architecture = QString::fromUtf8(flatpak_ref_get_arch(ref));
        m_branch = QString::fromUtf8(flatpak_ref_get_branch(ref));
        m_commit = QString::fromUtf8(flatpak_ref_get_commit(ref));
        m_latestCommit = m_commit;

        setFlatpakType(flatpak_ref_get_kind(ref));
    } else {
        m_packageName = m_appdata.id().remove(QLatin1String(".desktop"));
        m_architecture = QString();
        m_branch = QString();
        m_commit = QString();
        m_latestCommit = QString();

        setFlatpakType(FLATPAK_REF_KIND_APP);
    }
}

void FlatpakResource::updateFromInstalledRef(FlatpakInstalledRef *ref)
{
    if (ref) {
        m_latestCommit = QString::fromUtf8(flatpak_installed_ref_get_latest_commit(ref));
        m_installedSize = flatpak_installed_ref_get_installed_size(ref);
        updateFromMetadata(ref);
        setState(updatesAvailable() ? SoftwareResource::UpgradableState : SoftwareResource::InstalledState);
    } else {
        m_latestCommit = QString();
        m_runtime = QString();
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
    if (m_type != SoftwareResource::App)
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

QDir FlatpakResource::installationDir(FlatpakInstallation *installation)
{
    g_autoptr(GFile) path = flatpak_installation_get_path(installation);
    return QDir(QString::fromUtf8(g_file_get_path(path)));
}

void FlatpakResource::setFlatpakType(FlatpakRefKind kind)
{
    m_kind = kind;

    if (m_packageName.endsWith(QLatin1String(".Locale")))
        m_type = Liri::AppCenter::SoftwareResource::Localization;
    else if (m_packageName.endsWith(QLatin1String(".Debug")) ||
             m_packageName.endsWith(QLatin1String(".Sources")))
        m_type = Liri::AppCenter::SoftwareResource::Generic;
    else if (m_packageName.startsWith(QLatin1String("org.freedesktop.Platform.Icontheme.")) ||
             m_packageName.startsWith(QLatin1String("org.gtk.Gtk3theme.")) ||
             m_packageName.startsWith(QLatin1String("org.kde.KStyle.")) ||
             m_packageName.startsWith(QLatin1String("org.kde.WaylandDecoration.")))
        m_type = Liri::AppCenter::SoftwareResource::Theme;
    else if (m_packageName.contains(QLatin1String(".Extension.")))
        m_type = Liri::AppCenter::SoftwareResource::Addon;
    else {
        switch (kind) {
        case FLATPAK_REF_KIND_APP: {
            switch (m_appdata.kind()) {
            case AppStream::Component::KindAddon:
                m_type = Liri::AppCenter::SoftwareResource::Addon;
                break;
            case AppStream::Component::KindCodec:
                m_type = Liri::AppCenter::SoftwareResource::Codec;
                break;
            case AppStream::Component::KindDriver:
                m_type = Liri::AppCenter::SoftwareResource::Driver;
                break;
            case AppStream::Component::KindFirmware:
                m_type = Liri::AppCenter::SoftwareResource::Firmware;
                break;
            case AppStream::Component::KindFont:
                m_type = Liri::AppCenter::SoftwareResource::Font;
                break;
            case AppStream::Component::KindGeneric:
                m_type = Liri::AppCenter::SoftwareResource::Generic;
                break;
            case AppStream::Component::KindInputmethod:
                m_type = Liri::AppCenter::SoftwareResource::InputMethod;
                break;
            case AppStream::Component::KindLocalization:
                m_type = Liri::AppCenter::SoftwareResource::Localization;
                break;
            default:
                m_type = Liri::AppCenter::SoftwareResource::App;
                break;
            }
            break;
        case FLATPAK_REF_KIND_RUNTIME:
                m_type = Liri::AppCenter::SoftwareResource::Runtime;
                break;
            }
        }
    }
}

void FlatpakResource::setState(Liri::AppCenter::SoftwareResource::State state)
{
    m_state = state;
    Q_EMIT stateChanged();
}

void FlatpakResource::updateComponent()
{
    // Change log
    m_changeLog = m_appdata.description();
    m_changeLog.replace(QLatin1Char('\n'), QLatin1String("<br>"));

    // Icons
    const auto icons = m_appdata.icons();
    for (const auto &icon : icons) {
        if (icon.width() >= 128 && icon.height() >= 128)
            addKudo(SoftwareResource::HiDpiIconKudo);

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
    const auto screenshots = m_appdata.screenshots();
    for (const auto &screenshot : screenshots) {
        bool thumbnailDone = false;
        bool screenshotDone = false;

        const auto images = screenshot.images();
        for (const auto &image : images) {
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
    if (m_appdata.screenshots().size() > 0)
        addKudo(SoftwareResource::HasScreenshotsKudo);

    // Package information
    const auto bundles = m_appdata.bundles();
    for (const auto &bundle : bundles) {
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

    // Is last build less than a year ago?
    const auto releases = m_appdata.releases();
    for (const auto &release : releases) {
        if (release.timestamp().daysTo(QDateTime::currentDateTime()) < 365) {
            addKudo(SoftwareResource::RecentReleaseKudo);
            break;
        }
    }

    // Featured
    if (m_appdata.categories().contains(QStringLiteral("Featured")))
        addKudo(SoftwareResource::FeaturedRecommendedKudo);

    // FIXME: We should detect more kudos but the AppStream API is very limited
    //        For installed refs we detect the missing kudos with updateFromMetadata()
    //        We should write our XML parser, see #19
}

void FlatpakResource::updateFromMetadata(FlatpakInstalledRef *ref)
{
    g_autoptr(GCancellable) cancellable = g_cancellable_new();
    g_autoptr(GError) error = nullptr;

    g_autoptr(GBytes) bytes = flatpak_installed_ref_load_metadata(ref, cancellable, &error);
    if (!bytes) {
        qCWarning(lcFlatpakBackend, "Failed to load metadata for \"%s\": %s",
                  qPrintable(name()), error->message);
        return;
    }

    // Can't use QSettings because it needs a file name
    g_autoptr(GKeyFile) kf = g_key_file_new();
    if (!g_key_file_load_from_bytes(kf, bytes, G_KEY_FILE_NONE, &error)) {
        qCWarning(lcFlatpakBackend, "Failed to read metadata for \"%s\": %s",
                  qPrintable(name()), error->message);
        return;
    }

    // Runtime name
    g_autofree gchar *runtime =
            g_key_file_get_string(kf, "Application", "runtime", &error);
    if (!runtime) {
        qCWarning(lcFlatpakBackend, "Failed to parse metadata for \"%s\": %s",
                  qPrintable(name()), error->message);
        return;
    }
    m_runtime = QString::fromUtf8(runtime);

    /*
     * Is it secure?
     */

    bool secure = true;

    g_auto(GStrv) shared =
            g_key_file_get_string_list(kf, "Context", "shared", nullptr, nullptr);
    if (shared) {
        // SHM is not secure enough
        if (g_strv_contains((const gchar * const *) shared, "ipc"))
            secure = false;
    }

    g_auto(GStrv) sockets =
            g_key_file_get_string_list(kf, "Context", "sockets", nullptr, nullptr);
    if (sockets) {
        // X11 is not secure enough
        if (g_strv_contains((const gchar * const *) sockets, "x11"))
            secure = false;
    }

    g_auto(GStrv) filesystems =
            g_key_file_get_string_list(kf, "Context", "filesystems", nullptr, nullptr);
    if (filesystems) {
        // Secure apps should be using portals
        if (g_strv_contains((const gchar * const *) filesystems, "home"))
            secure = false;
    }

    if (secure)
        addKudo(SoftwareResource::SandboxedSecureKudo);

    /*
     * Kudos
     */

    // ISO 639 name of the current language
    const auto curLang = QLocale::system().name();

    QByteArray appdata = loadAppData(ref);
    if (!appdata.isEmpty()) {
        QXmlStreamReader xml(appdata);
        while (!xml.atEnd()) {
            xml.readNext();

            if (xml.name().compare(QStringLiteral("lang")) == 0) {
                auto lang = xml.readElementText();
                if (lang != curLang)
                    continue;
                const auto attrs = xml.attributes();
                for (const auto &attr : attrs) {
                    if (attr.name().compare(QStringLiteral("percentage")) == 0) {
                        bool ok = false;
                        int value = attr.value().toInt(&ok);

                        if (ok && value >= 50) {
                            addKudo(SoftwareResource::MyLanguageKudo);
                            break;
                        }
                    }
                }
            } else if (xml.name().compare(QStringLiteral("keyword")) == 0) {
                addKudo(SoftwareResource::HasKeywordsKudo);
            } else if (xml.name().compare(QStringLiteral("kudo")) == 0) {
                auto value = xml.readElementText();

                if (value == QStringLiteral("SearchProvider"))
                    addKudo(SoftwareResource::SearchProviderKudo);
                else if (value == QStringLiteral("UserDocs"))
                    addKudo(SoftwareResource::InstallsUserDocsKudo);
                else if (value == QStringLiteral("AppMenu"))
                    addKudo(SoftwareResource::AppMenuKudo);
                else if (value == QStringLiteral("ModernToolkit"))
                    addKudo(SoftwareResource::ModernToolkitKudo);
                else if (value == QStringLiteral("Notifications"))
                    addKudo(SoftwareResource::UsesNotificationsKudo);
                else if (value == QStringLiteral("HighContrast"))
                    addKudo(SoftwareResource::HighContrastKudo);
                else if (value == QStringLiteral("HiDpiIcon"))
                    addKudo(SoftwareResource::HiDpiIconKudo);
                else if (value == QStringLiteral("GnomeSoftware::popular"))
                    addKudo(SoftwareResource::FeaturedRecommendedKudo);
            }
        }
    }
}
