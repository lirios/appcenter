// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWAREMANAGER_H
#define LIRIAPPCENTERSOFTWAREMANAGER_H

#include <LiriAppCenter/Rating>
#include <LiriAppCenter/Review>
#include <LiriAppCenter/ReviewsModel>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SourcesModel>

namespace Liri {

namespace AppCenter {

class ResourceProxy;
class SoftwareManagerPrivate;

class LIRIAPPCENTER_EXPORT SoftwareManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SourcesModel *sourcesModel READ sourcesModel CONSTANT)
    Q_PROPERTY(bool hasUpdates READ hasUpdates NOTIFY updatesAvailable)
    Q_PROPERTY(uint updatesCount READ updatesCount NOTIFY updatesAvailable)
    Q_DECLARE_PRIVATE(SoftwareManager)
    Q_DISABLE_COPY(SoftwareManager)
public:
    explicit SoftwareManager(QObject *parent = nullptr);
    ~SoftwareManager();

    SourcesModel *sourcesModel() const;

    void addResource(const QString &id, SoftwareResource *resource);
    void removeResource(const QString &id, SoftwareResource *resource);

    QList<ResourceProxy *> resourceProxies() const;

    Q_INVOKABLE bool addSource(const QString &name);
    Q_INVOKABLE bool removeSource(Liri::AppCenter::SoftwareSource *source);

    SoftwareResources updates() const;
    bool hasUpdates() const;
    uint updatesCount() const;

public Q_SLOTS:
    void initialize();
    void checkForUpdates();

Q_SIGNALS:
    void initializationCompleted();
    void sourceAdded(Liri::AppCenter::SoftwareSource *source);
    void sourceAddFailed(const QString &name, const QString &errorMessage);
    void updatesAvailable(uint count);
    void resourceProxiesAdded(QList<Liri::AppCenter::ResourceProxy *> proxies);
    void resourceProxyRemoved(Liri::AppCenter::ResourceProxy *proxy);
    void ratingAdded(Liri::AppCenter::Rating *rating);
    void reviewAdded(Liri::AppCenter::Review *review);

private:
    SoftwareManagerPrivate *const d_ptr;
};

} // namespace AppCenter

} // namesapce Liri

#endif // LIRIAPPCENTERSOFTWAREMANAGER_H
