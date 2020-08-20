// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKRESOURCE_H
#define LIRI_FLATPAKRESOURCE_H

#include <QDir>
#include <QUrl>
#include <QVector>

#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/Transaction>

#include <AppStreamQt/component.h>

#include "flatpakpluginbase.h"

class FlatpakResource : public Liri::AppCenter::SoftwareResource
{
    Q_OBJECT
public:
    explicit FlatpakResource(Liri::AppCenter::SoftwareManager *manager,
                             const AppStream::Component &component,
                             FlatpakInstallation *installation,
                             QObject *parent = nullptr);

    // Everything that makes this resource unique is here
    struct Key {
        Liri::AppCenter::SoftwareResource::Type type = Liri::AppCenter::SoftwareResource::App;
        FlatpakInstallation *installation = nullptr;
        QString desktopId;
        QString origin;
        QString branch;
        QString architecture;

        bool operator!=(const Key &other) const
        {
            return !operator==(other);
        }

        bool operator==(const Key &other) const
        {
            return &other == this || (
                        other.type == type &&
                        other.installation == installation &&
                        other.desktopId == desktopId &&
                        other.origin == origin &&
                        other.branch == branch &&
                        other.architecture == architecture
                        );
        }
    };

    static FlatpakResource::Key keyFromInstalledRef(FlatpakInstallation *installation, FlatpakInstalledRef *ref);

    Key key() const;

    AppStream::Component component() const;
    FlatpakInstallation *installation() const;
    FlatpakRefKind kind() const;

    Liri::AppCenter::SoftwareResource::Type type() const override;
    Liri::AppCenter::SoftwareResource::State state() const override;

    QString appId() const override;

    QString name() const override;
    QString summary() const override;
    QString description() const override;

    QString packageName() const override;
    QString architecture() const override;

    QString license() const override;

    QString origin() const override;

    QStringList categories() const override;

    QUrl homepageUrl() const override;
    QUrl bugtrackerUrl() const override;
    QUrl faqUrl() const override;
    QUrl helpUrl() const override;
    QUrl donationUrl() const override;
    QUrl translateUrl() const override;

    QString installedVersion() const override;
    QString availableVersion() const override;

    bool updatesAvailable() const override;

    quint64 downloadSize() const override;
    quint64 installedSize() const override;

    QString changeLog() const override;

    QStringList iconNames() const override;
    QVector<QUrl> iconUrls() const override;

    QVector<Liri::AppCenter::Image> thumbnails() const override;
    QVector<Liri::AppCenter::Image> screenshots() const override;

    bool isLocalized() const override;

    Q_INVOKABLE bool launch() const override;
    Q_INVOKABLE Liri::AppCenter::Transaction *install() override;
    Q_INVOKABLE Liri::AppCenter::Transaction *uninstall() override;
    Q_INVOKABLE Liri::AppCenter::Transaction *update() override;

    QDir installationDir() const;
    QDir exportsDir() const;

    QString runtime() const;
    QString branch() const;
    QString commit() const;
    QString ref() const;

    void updateFromResource(FlatpakResource *resource);
    void updateFromRemote(FlatpakRemote *remote);
    void updateFromRef(FlatpakRef *ref);
    void updateFromInstalledRef(FlatpakInstalledRef *ref);
    void updateDownloadSize(FlatpakResource *remoteResource, FlatpakRef *ref);

    static QDir installationDir(FlatpakInstallation *installation);

private:
    FlatpakResource::Key m_key;
    Liri::AppCenter::SoftwareResource::State m_state = Liri::AppCenter::SoftwareResource::NotInstalledState;
    AppStream::Component m_appdata;
    FlatpakRefKind m_kind = FLATPAK_REF_KIND_APP;
    QString m_packageName;
    QString m_runtime;
    QString m_commit;
    QString m_latestCommit;
    QString m_changeLog;
    QStringList m_stockIcons;
    QVector<QUrl> m_icons;
    QString m_iconsPath;
    QVector<Liri::AppCenter::Image> m_thumbnails;
    QVector<Liri::AppCenter::Image> m_screenshots;
    quint64 m_downloadSize = 0;
    quint64 m_installedSize = 0;

    void setFlatpakType(FlatpakRefKind kind);
    void setState(State state);
    void updateComponent(const AppStream::Component &component);
    void updateFromMetadata(FlatpakInstalledRef *ref);

    friend class FlatpakBackend;
};

inline uint qHash(const FlatpakResource::Key &key)
{
    return qHash(key.type)
            ^ qHash(key.installation)
            ^ qHash(key.desktopId)
            ^ qHash(key.origin)
            ^ qHash(key.branch)
            ^ qHash(key.architecture);
}

#endif // LIRI_FLATPAKRESOURCE_H
