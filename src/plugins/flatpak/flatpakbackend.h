// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKBACKEND_H
#define LIRI_FLATPAKBACKEND_H

#include <QLoggingCategory>

#include <LiriAppCenter/Backend>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SourcesModel>

#include "flatpakpluginbase.h"
#include "flatpakresource.h"

Q_DECLARE_LOGGING_CATEGORY(lcFlatpakBackend)

class FlatpakBackend : public Liri::AppCenter::Backend
{
    Q_OBJECT
public:
    explicit FlatpakBackend(QObject *parent = nullptr);
    ~FlatpakBackend();

    void initialize(Liri::AppCenter::SoftwareManager *manager) override;

    void listSources() override;
    void listAvailableApps() override;
    void listInstalledApps() override;
    void checkForUpdates() override;

    QVector<Liri::AppCenter::SoftwareResource *> updates() const override;

    bool addSource(const QString &source) override;
    bool removeSource(Liri::AppCenter::SoftwareSource *source) override;

    Liri::AppCenter::SoftwareResource *installAppFromPath(const QString &filePath) override;

    static GPtrArray *listUpdates(FlatpakInstallation *installation);

private:
    Liri::AppCenter::SoftwareManager *m_manager = nullptr;
    GCancellable *m_cancellable = nullptr;
    QVector<FlatpakInstallation *> m_installations;
    QVector<GFileMonitor *> m_installationMonitors;
    FlatpakInstallation *m_userInstallation = nullptr;
    QHash<FlatpakResource::Key, FlatpakResource *> m_resources;

    FlatpakResource *findResourceFromInstalledRef(FlatpakInstallation *installation, FlatpakInstalledRef *ref) const;
    FlatpakResource *findRuntimeResource(const QString &runtime);

    bool extractRepositories(FlatpakInstallation *installation);
    bool listAvailableApps(FlatpakInstallation *installation);
    bool listInstalledApps(FlatpakInstallation *installation);

    FlatpakResource *installFromFlatpakRef(const QString &filePath);

    bool addLocalSource(const QString &name, const QUrl &url);

    void fetchAppStreamMetadata(FlatpakInstallation *installation, FlatpakRemote *remote);

    bool checkLocalUpdatesForInstallation(FlatpakInstallation *installation);
    void checkUpdatesForInstallation(FlatpakInstallation *installation);
    void updatesFetched(FlatpakInstallation *installation, GPtrArray *refs);

private Q_SLOTS:
    void addAppsFromRemote(FlatpakInstallation *installation, FlatpakRemote *remote);
};

#endif // LIRI_FLATPAKBACKENS_H
