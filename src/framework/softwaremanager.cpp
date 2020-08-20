// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>

#include "backend.h"
#include "backendplugin.h"
#include "resourcesmodel.h"
#include "softwaremanager.h"
#include "softwaremanager_p.h"
#include "sourcesmodel.h"

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
    qRegisterMetaType<ResourcesModel *>("ResourcesModel*");
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

bool SoftwareManager::addSource(const QString &name)
{
    Q_D(SoftwareManager);

    for (auto backend : d->backends) {
        if (backend->addSource(name))
            return true;
    }

    return false;
}

bool SoftwareManager::removeSource(SoftwareSource *source)
{
    Q_D(SoftwareManager);

    for (auto backend : d->backends) {
        if (backend->removeSource(source))
            return true;
    }

    return false;
}

SoftwareResources SoftwareManager::updates() const
{
    Q_D(const SoftwareManager);

    SoftwareResources list;

    for (auto backend : d->backends)
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

    // Find all backends
    for (const QString &path : QCoreApplication::libraryPaths()) {
        QDir pluginsPath(QDir(path).absoluteFilePath(QLatin1String("liri/appcenter")));

        for (const QString &fileName : pluginsPath.entryList(QDir::Files)) {
            QPluginLoader loader(pluginsPath.absoluteFilePath(fileName));
            BackendPlugin *plugin = qobject_cast<BackendPlugin *>(loader.instance());
            if (!plugin)
                continue;
            Backend *backend = qobject_cast<Backend *>(plugin->create());
            if (backend) {
                d->backends.append(backend);

                connect(backend, &Backend::updatesAvailable, this, [this, d](uint count) {
                    d->updatesCount = count;
                    Q_EMIT updatesAvailable(count);
                });

                backend->initialize(this);
                backend->listSources();
                backend->listAvailableApps();
                backend->listInstalledApps();
                backend->checkForUpdates();
            } else {
                loader.unload();
            }
        }
    }
}

void SoftwareManager::checkForUpdates()
{
    Q_D(SoftwareManager);

    for (auto backend : d->backends)
        backend->checkForUpdates();
}

} // namespace AppCenter

} // namespace Liri
