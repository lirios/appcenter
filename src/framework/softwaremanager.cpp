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

#include <QCoreApplication>
#include <QDir>
#include <QPluginLoader>
#include <QTimer>

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

    // Check for updates every single day
    QTimer *timer = new QTimer(this);
    timer->setInterval(24 * 60 * 60 * 1000);
    connect(timer, &QTimer::timeout, this, &SoftwareManager::checkForUpdates);
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

QVector<SoftwareResource *> SoftwareManager::updates() const
{
    Q_D(const SoftwareManager);

    QVector<SoftwareResource *> list;

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
