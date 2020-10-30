// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "resourceproxy_p.h"
#include "softwaremanager_p.h"
#include "sourcesmodel_p.h"

namespace Liri {

namespace AppCenter {

/*
 * ResourceProxyPrivate
 */

ResourceProxyPrivate::ResourceProxyPrivate()
    : sourcesModel(new SourcesModel())
{
}

ResourceProxyPrivate::~ResourceProxyPrivate()
{
    sourcesModel->deleteLater();
}

/*
 * ResourceProxy
 */

ResourceProxy::ResourceProxy(QObject *parent)
    : QObject(parent)
    , d_ptr(new ResourceProxyPrivate())
{
}

ResourceProxy::~ResourceProxy()
{
    delete d_ptr;
}

QString ResourceProxy::id() const
{
    Q_D(const ResourceProxy);
    return d->id;
}

SoftwareSource *ResourceProxy::defaultSource() const
{
    Q_D(const ResourceProxy);
    return d->defaultSource;
}

void ResourceProxy::setDefaultSource(SoftwareSource *source)
{
    Q_D(ResourceProxy);

    if (!source) {
        qCWarning(lcAppCenter, "Unable to set ResourceProxy::defaultSource to null");
        return;
    }

    auto *oldSource = d->defaultSource;
    d->defaultSource = source;
    if (oldSource != d->defaultSource)
        Q_EMIT defaultSourceChanged();

    auto *oldRes = d->selectedResource;
    SoftwareResource *newRes = nullptr;
    for (auto *resource : qAsConst(d->resources)) {
        if (resource->source() == source) {
            newRes = resource;
            break;
        }
    }
    if (oldRes != newRes) {
        d->selectedResource = newRes;
        Q_EMIT selectedResourceChanged();

        if (oldRes)
            disconnect(oldRes, &SoftwareResource::dataChanged,
                       this, &ResourceProxy::dataChanged);
        if (newRes)
            connect(newRes, &SoftwareResource::dataChanged,
                    this, &ResourceProxy::dataChanged);
    }

    if (oldSource != d->defaultSource || oldRes != newRes)
        Q_EMIT dataChanged();
}

int ResourceProxy::defaultSourceIndex() const
{
    Q_D(const ResourceProxy);
    return SourcesModelPrivate::get(d->sourcesModel)->sources.indexOf(d->defaultSource);
}

QList<SoftwareSource *> ResourceProxy::sources() const
{
    Q_D(const ResourceProxy);
    return SourcesModelPrivate::get(d->sourcesModel)->sources;
}

SourcesModel *ResourceProxy::sourcesModel() const
{
    Q_D(const ResourceProxy);
    return d->sourcesModel;
}

SoftwareResource *ResourceProxy::selectedResource() const
{
    Q_D(const ResourceProxy);
    return d->selectedResource;
}

bool ResourceProxy::hasResource(SoftwareResource *resource) const
{
    Q_D(const ResourceProxy);
    return d->resources.contains(resource);
}

QList<SoftwareResource *> ResourceProxy::resources() const
{
    Q_D(const ResourceProxy);
    return d->resources;
}

void ResourceProxy::addResource(SoftwareResource *resource)
{
    Q_D(ResourceProxy);

    if (!d->resources.contains(resource)) {
        d->resources.append(resource);
        d->sourcesModel->addSource(resource->source());
        connect(resource, &SoftwareResource::dataChanged,
                this, &ResourceProxy::dataChanged);
    }

    if (!d->defaultSource)
        setDefaultSource(resource->source());
}

bool ResourceProxy::removeResource(SoftwareResource *resource)
{
    Q_D(ResourceProxy);

    if (d->resources.removeOne(resource)) {
        disconnect(resource, &SoftwareResource::dataChanged,
                   this, &ResourceProxy::dataChanged);
        d->sourcesModel->removeSource(resource->source());
        if (d->selectedResource == resource)
            setDefaultSource(d->resources.first()->source());

        return true;
    }

    return false;
}

SoftwareResource::Type ResourceProxy::type() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->type();
    return SoftwareResource::Generic;
}

SoftwareResource::State ResourceProxy::state() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->state();
    return SoftwareResource::BrokenState;
}

QString ResourceProxy::name() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->name();
    return QString();
}

QString ResourceProxy::summary() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->summary();
    return QString();
}

QString ResourceProxy::description() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->description();
    return QString();
}

QString ResourceProxy::iconName() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->iconName();
    return QString();
}

QUrl ResourceProxy::iconUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->iconUrl();
    return QUrl();
}

QString ResourceProxy::packageName() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->packageName();
    return QString();
}

QString ResourceProxy::architecture() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->architecture();
    return QString();
}

QString ResourceProxy::license() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->license();
    return QString();
}

QString ResourceProxy::category() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->category();
    return QString();
}

QUrl ResourceProxy::homepageUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->homepageUrl();
    return QUrl();
}

QUrl ResourceProxy::bugtrackerUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->bugtrackerUrl();
    return QUrl();
}

QUrl ResourceProxy::faqUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->faqUrl();
    return QUrl();
}

QUrl ResourceProxy::helpUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->helpUrl();
    return QUrl();
}

QUrl ResourceProxy::donationUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->donationUrl();
    return QUrl();
}

QUrl ResourceProxy::translateUrl() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->translateUrl();
    return QUrl();
}

QString ResourceProxy::version() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->version();
    return QString();
}

QString ResourceProxy::installedVersion() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->installedVersion();
    return QString();
}

QString ResourceProxy::availableVersion() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->availableVersion();
    return QString();
}

bool ResourceProxy::updatesAvailable() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->updatesAvailable();
    return false;
}

quint64 ResourceProxy::downloadSize() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->downloadSize();
    return 0;
}

quint64 ResourceProxy::installedSize() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->installedSize();
    return 0;
}

quint64 ResourceProxy::size() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->size();
    return 0;
}

QString ResourceProxy::changeLog() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->changeLog();
    return QString();
}

bool ResourceProxy::isInstalled() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->isInstalled();
    return false;
}

bool ResourceProxy::isLocalized() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->isInstalled();
    return false;
}

SoftwareResource::Kudos ResourceProxy::kudos() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->kudos();
    return SoftwareResource::Kudos();
}

uint ResourceProxy::kudosPercentage() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->kudosPercentage();
    return 0;
}

Rating *ResourceProxy::rating() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->rating();
    return nullptr;
}

bool ResourceProxy::hasKudo(SoftwareResource::Kudo kudo) const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->hasKudo(kudo);
    return false;
}

bool ResourceProxy::launch() const
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->launch();
    return false;
}

Transaction *ResourceProxy::install()
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->install();
    return nullptr;
}

Transaction *ResourceProxy::uninstall()
{
    Q_D(const ResourceProxy);
    if (d->selectedResource)
        return d->selectedResource->uninstall();
    return nullptr;
}

Transaction *ResourceProxy::update()
{
    Q_D(const ResourceProxy);

    if (d->selectedResource)
        return d->selectedResource->update();
    return nullptr;
}

bool ResourceProxy::operator==(const ResourceProxy &other)
{
    return other.id() == id();
}

} // namespace AppCenter

} // namespace Liri
