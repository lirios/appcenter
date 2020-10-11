// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>

#include "review.h"
#include "softwaremanager_p.h"
#include "softwareresource_p.h"

namespace Liri {

namespace AppCenter {

SoftwareManagerPrivate::SoftwareManagerPrivate()
    : sourcesModel(new SourcesModel())
    , resourcesModel(new ResourcesModel())
{
}

SoftwareManagerPrivate::~SoftwareManagerPrivate()
{
    delete sourcesModel;
    delete resourcesModel;
}

SoftwareManager::SoftwareManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new SoftwareManagerPrivate())
{
    qRegisterMetaType<SoftwareSource *>("SoftwareSource*");
    qRegisterMetaType<SoftwareResource *>("SoftwareResource*");
    qRegisterMetaType<SourcesModel *>("SourcesModel*");
    qRegisterMetaType<Rating *>("Rating*");
    qRegisterMetaType<Review *>("Review*");
    qRegisterMetaType<ResourcesModel *>("ResourcesModel*");
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

ResourcesModel *SoftwareManager::resourcesModel() const
{
    Q_D(const SoftwareManager);
    return d->resourcesModel;
}

void SoftwareManager::addResource(SoftwareResource *resource)
{
    Q_D(SoftwareManager);

    if (!d->resources.contains(resource))
        d->resources.append(resource);

    d->resourcesModel->addResource(resource);
}

void SoftwareManager::removeResource(SoftwareResource *resource)
{
    Q_D(SoftwareManager);

    d->resources.removeOne(resource);
    d->resourcesModel->addResource(resource);
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
