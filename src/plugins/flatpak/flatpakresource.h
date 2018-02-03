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

#ifndef FLATPAKRESOURCE_H
#define FLATPAKRESOURCE_H

#include <QDir>
#include <QUrl>
#include <QVector>

#include <LiriAppCenter/SoftwareResource>

#include <AppStreamQt/component.h>

#include "flatpakpluginbase.h"

class FlatpakResource : public Liri::AppCenter::SoftwareResource
{
    Q_OBJECT
public:
    explicit FlatpakResource(const AppStream::Component &component,
                             FlatpakInstallation *installation,
                             QObject *parent = nullptr);

    AppStream::Component component() const;
    FlatpakInstallation *installation() const;
    FlatpakRefKind kind() const;

    Liri::AppCenter::SoftwareResource::Type type() const override;
    Liri::AppCenter::SoftwareResource::State state() const override;

    QString name() const override;
    QString summary() const override;
    QString description() const override;

    QString desktopId() const;
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
    Q_INVOKABLE bool install() override;
    Q_INVOKABLE bool uninstall() override;
    Q_INVOKABLE bool update() override;

    QDir installationDir() const;
    QDir exportsDir() const;

    QString runtime() const;
    QString branch() const;
    QString commit() const;

    void updateFromResource(FlatpakResource *resource);
    void updateFromRemote(FlatpakRemote *remote);
    void updateFromRef(FlatpakRef *ref);
    void updateFromInstalledRef(FlatpakInstalledRef *ref);
    bool updateFromMetadata();

    static QDir installationDir(FlatpakInstallation *installation);

private:
    Liri::AppCenter::SoftwareResource::Type m_type = Liri::AppCenter::SoftwareResource::App;
    Liri::AppCenter::SoftwareResource::State m_state = Liri::AppCenter::SoftwareResource::NotInstalledState;
    AppStream::Component m_appdata;
    FlatpakInstallation *m_installation = nullptr;
    FlatpakRefKind m_kind = FLATPAK_REF_KIND_APP;
    QString m_desktopId;
    QString m_packageName;
    QString m_architecture;
    QString m_origin;
    QString m_runtime;
    QString m_branch;
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
};

#endif // FLATPAKRESOURCE_H
