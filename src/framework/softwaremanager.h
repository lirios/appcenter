// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWAREMANAGER_H
#define LIRIAPPCENTERSOFTWAREMANAGER_H

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SourcesModel>

namespace Liri {

namespace AppCenter {

class SoftwareManagerPrivate;

class LIRIAPPCENTER_EXPORT SoftwareManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SourcesModel *sourcesModel READ sourcesModel CONSTANT)
    Q_PROPERTY(ResourcesModel *resourcesModel READ resourcesModel CONSTANT)
    Q_PROPERTY(bool hasUpdates READ hasUpdates NOTIFY updatesAvailable)
    Q_PROPERTY(uint updatesCount READ updatesCount NOTIFY updatesAvailable)
    Q_DECLARE_PRIVATE(SoftwareManager)
    Q_DISABLE_COPY(SoftwareManager)
public:
    explicit SoftwareManager(QObject *parent = nullptr);
    ~SoftwareManager();

    SourcesModel *sourcesModel() const;
    ResourcesModel *resourcesModel() const;

    Q_INVOKABLE bool addSource(const QString &name);
    Q_INVOKABLE bool removeSource(SoftwareSource *source);

    SoftwareResources updates() const;
    bool hasUpdates() const;
    uint updatesCount() const;

public Q_SLOTS:
    void initialize();
    void checkForUpdates();

Q_SIGNALS:
    void initializationCompleted();
    void sourceAdded(SoftwareSource *source);
    void sourceAddFailed(const QString &name, const QString &errorMessage);
    void updatesAvailable(uint count);

private:
    SoftwareManagerPrivate *const d_ptr;
};

} // namespace AppCenter

} // namesapce Liri

#endif // LIRIAPPCENTERSOFTWAREMANAGER_H
