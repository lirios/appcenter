// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>

#include "resourcesmodel.h"
#include "review.h"
#include "softwaremanager_p.h"
#include "softwareresource_p.h"

Q_LOGGING_CATEGORY(lcAppCenter, "liri.appcenter")

namespace Liri {

namespace AppCenter {

/*
 * SoftwareManagerPrivate
 */

SoftwareManagerPrivate::SoftwareManagerPrivate(SoftwareManager *self)
    : sourcesModel(new SourcesModel())
    , q_ptr(self)
{
}

SoftwareManagerPrivate::~SoftwareManagerPrivate()
{
    delete sourcesModel;
}

void SoftwareManagerPrivate::populate()
{
    Q_Q(SoftwareManager);
    Q_EMIT q->resourceProxiesAdded(proxies);
}

/*
 * SoftwareManager
 */

SoftwareManager::SoftwareManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareManagerPrivate(this))
{
    qRegisterMetaType<SoftwareSource *>("SoftwareSource*");
    qRegisterMetaType<SoftwareResource *>("SoftwareResource*");
    qRegisterMetaType<SoftwareResource::State>("SoftwareResource::State");
    qRegisterMetaType<SoftwareResource::Kudo>("SoftwareResource::Kudo");
    qRegisterMetaType<SoftwareResource::Kudos>("SoftwareResource::Kudos");
    qRegisterMetaType<SourcesModel *>("SourcesModel*");
    qRegisterMetaType<Rating *>("Rating*");
    qRegisterMetaType<Review *>("Review*");
    qRegisterMetaType<ResourcesModel *>("ResourcesModel*");
    qRegisterMetaType<ResourceProxy *>("ResourceProxy*");
    qRegisterMetaType<QList<ResourceProxy *>>("QList<ResourceProxy*>");
    qRegisterMetaType<ReviewsModel *>("ReviewsModel*");
}

SoftwareManager::~SoftwareManager()
{
    delete d_ptr;
}

SourcesModel *SoftwareManager::sourcesModel() const
{
    Q_D(const SoftwareManager);
    return d->sourcesModel;
}

void SoftwareManager::addResource(const QString &id, SoftwareResource *resource)
{
    Q_D(SoftwareManager);

    auto *proxy = d->proxiesMap.value(id);
    if (!proxy) {
        proxy = new ResourceProxy(this);
        d->proxiesMap[id] = proxy;
        d->proxies.append(proxy);
    }

    if (!proxy->hasResource(resource))
        proxy->addResource(resource);

    if (!d->resources.contains(resource))
        d->resources.append(resource);
}

void SoftwareManager::removeResource(const QString &id, SoftwareResource *resource)
{
    Q_D(SoftwareManager);

    if (d->proxiesMap.contains(id)) {
        auto *proxy = d->proxiesMap[id];

        proxy->removeResource(resource);
        if (proxy->resources().size() == 0) {
            Q_EMIT resourceProxyRemoved(proxy);
            d->proxies.removeOne(proxy);
            d->proxiesMap.take(id)->deleteLater();
        }
    }

    d->resources.removeOne(resource);
}

QList<ResourceProxy *> SoftwareManager::resourceProxies() const
{
    Q_D(const SoftwareManager);
    return d->proxies;
}

bool SoftwareManager::addSource(const QString &name)
{
    Q_D(SoftwareManager);

    for (auto backend : qAsConst(d->resourcesBackends)) {
        if (backend->addSource(name))
            return true;
    }

    return false;
}

bool SoftwareManager::removeSource(SoftwareSource *source)
{
    Q_D(SoftwareManager);

    for (auto backend : qAsConst(d->resourcesBackends)) {
        if (backend->removeSource(source))
            return true;
    }

    return false;
}

SoftwareResources SoftwareManager::updates() const
{
    Q_D(const SoftwareManager);

    SoftwareResources list;

    for (auto backend : qAsConst(d->resourcesBackends))
        list += backend->updates();

    return list;
}

bool SoftwareManager::hasUpdates() const
{
    Q_D(const SoftwareManager);
    return d->updatesCount > 0;
}

uint SoftwareManager::updatesCount() const
{
    Q_D(const SoftwareManager);
    return d->updatesCount;
}

void SoftwareManager::initialize()
{
    Q_D(SoftwareManager);

    const auto paths = QCoreApplication::libraryPaths();

    // Find all resource backends and add them to the list
    for (const QString &path : paths) {
        QDir pluginsPath(QDir(path).absoluteFilePath(QLatin1String("liri/appcenter/resources")));

        const auto files = pluginsPath.entryList(QDir::Files);
        for (const QString &fileName : files) {
            QPluginLoader loader(pluginsPath.absoluteFilePath(fileName));

            auto *plugin = qobject_cast<BackendPlugin *>(loader.instance());
            if (plugin) {
                auto *backend = qobject_cast<Backend *>(plugin->create(this));
                if (backend) {
                    d->resourcesBackends.append(backend);

                    // Emit a signal with the total available updates count
                    connect(backend, &Backend::updatesAvailable, this, [this, d](uint count) {
                        d->updatesCount = count;
                        Q_EMIT updatesAvailable(count);
                    });
                } else {
                    loader.unload();
                }
            }
        }
    }

    // Find all reviews backends and add them to the list
    for (const QString &path : paths) {
        QDir pluginsPath(QDir(path).absoluteFilePath(QLatin1String("liri/appcenter/reviews")));

        const auto files = pluginsPath.entryList(QDir::Files);
        for (const QString &fileName : files) {
            QPluginLoader loader(pluginsPath.absoluteFilePath(fileName));

            auto *plugin = qobject_cast<ReviewsBackendPlugin *>(loader.instance());
            if (plugin) {
                auto *backend = qobject_cast<ReviewsBackend *>(plugin->create(this));
                if (backend) {
                    d->reviewsBackends.append(backend);

                    // Add ratings to software resources
                    connect(backend, &ReviewsBackend::ratingsReady, this, [d, backend] {
                        for (auto *resource : qAsConst(d->resources)) {
                            auto *rating = backend->ratingForSofwareResource(resource);
                            if (rating)
                                SoftwareResourcePrivate::get(resource)->setRating(rating);
                        }
                    });

                    // Relay review signals
                    connect(backend, &ReviewsBackend::reviewAdded,
                            this, &SoftwareManager::reviewAdded);
                } else {
                    loader.unload();
                }
            }
        }
    }

    // Initialize them all
    for (auto *backend : qAsConst(d->resourcesBackends)) {
        backend->initialize();
        backend->listSources();
        backend->listAvailableApps();
        backend->listInstalledApps();
        backend->checkForUpdates();
    }
    for (auto *backend : qAsConst(d->reviewsBackends)) {
        backend->initialize();
        backend->fetchRatings();
    }
}

void SoftwareManager::checkForUpdates()
{
    Q_D(SoftwareManager);

    for (auto backend : qAsConst(d->resourcesBackends))
        backend->checkForUpdates();
}

} // namespace AppCenter

} // namespace Liri
