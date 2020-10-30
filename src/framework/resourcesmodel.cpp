// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "softwareresource.h"
#include "rating.h"
#include "resourcesmodel_p.h"
#include "softwaremanager_p.h"
#include "softwaresource.h"

namespace Liri {

namespace AppCenter {

/*
 * ResourcesModelPrivate
 */

ResourcesModelPrivate::ResourcesModelPrivate(ResourcesModel *self)
    : q_ptr(self)
{
}

void ResourcesModelPrivate::addProxies(QList<ResourceProxy *> list)
{
    Q_Q(ResourcesModel);

    for (auto *proxy : qAsConst(list)) {
        auto connection = q->connect(proxy, &ResourceProxy::dataChanged, q, [this, proxy] {
            handleProxyChanged(proxy);
        });
        connections[proxy] = connection;
        proxies.append(proxy);
        handleProxyChanged(proxy);
    }
}

void ResourcesModelPrivate::handlePopulated(QList<ResourceProxy *> list)
{
    Q_Q(ResourcesModel);

    q->beginInsertRows(QModelIndex(), proxies.size(), proxies.size() + list.size() - 1);
    addProxies(list);
    q->endInsertRows();
}

void ResourcesModelPrivate::handleProxyRemoved(ResourceProxy *proxy)
{
    Q_Q(ResourcesModel);

    int row = proxies.indexOf(proxy);

    q->beginRemoveRows(QModelIndex(), row, row);
    if (connections.contains(proxy)) {
        QObject::disconnect(connections[proxy]);
        connections.remove(proxy);
    }
    proxies.removeOne(proxy);
    q->endRemoveRows();
}

void ResourcesModelPrivate::handleProxyChanged(ResourceProxy *proxy)
{
    Q_Q(ResourcesModel);

    auto row = proxies.indexOf(proxy);
    if (row < 0 || row >= proxies.size())
        return;

    auto modelIndex = q->index(row);
    if (modelIndex.isValid())
        Q_EMIT q->dataChanged(modelIndex, modelIndex);
}

/*
 * ResourcesModel
 */

ResourcesModel::ResourcesModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ResourcesModelPrivate(this))
{
}

ResourcesModel::~ResourcesModel()
{
    delete d_ptr;
}

SoftwareManager *ResourcesModel::manager() const
{
    Q_D(const ResourcesModel);
    return d->manager;
}

void ResourcesModel::setManager(SoftwareManager *manager)
{
    Q_D(ResourcesModel);

    if (d->manager == manager)
        return;

    if (d->initialized) {
        qCWarning(lcAppCenter, "Cannot set ResourcesModel::manager after initialization!");
        return;
    }

    d->initialized = true;

    if (d->manager) {
        disconnect(manager, SIGNAL(resourceProxiesAdded(QList<Liri::AppCenter::ResourceProxy*>)),
                   this, SLOT(handlePopulated(QList<Liri::AppCenter::ResourceProxy*>)));
        disconnect(manager, SIGNAL(resourceProxyRemoved(Liri::AppCenter::ResourceProxy*)),
                   this, SLOT(handleProxyRemoved(Liri::AppCenter::ResourceProxy*)));
    }

    d->manager = manager;
    connect(manager, SIGNAL(resourceProxiesAdded(QList<Liri::AppCenter::ResourceProxy*>)),
            this, SLOT(handlePopulated(QList<Liri::AppCenter::ResourceProxy*>)));
    connect(manager, SIGNAL(resourceProxyRemoved(Liri::AppCenter::ResourceProxy*)),
            this, SLOT(handleProxyRemoved(Liri::AppCenter::ResourceProxy*)));
    Q_EMIT managerChanged();
}

QHash<int, QByteArray> ResourcesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ProxyRole, "proxy");
    roles.insert(TypeRole, QByteArrayLiteral("type"));
    roles.insert(StateRole, QByteArrayLiteral("state"));
    roles.insert(AppIdRole, "appId");
    roles.insert(NameRole, QByteArrayLiteral("name"));
    roles.insert(SummaryRole, "summary");
    roles.insert(DescriptionRole, "description");
    roles.insert(IconNameRole, "iconName");
    roles.insert(IconUrlRole, "iconUrl");
    roles.insert(PackageNameRole, "packageName");
    roles.insert(ArchitectureRole, "architecture");
    roles.insert(LicenseRole, "license");
    roles.insert(CategoryRole, "category");
    roles.insert(HomePageUrlRole, "homepageUrl");
    roles.insert(BugTrackerUrlRole, "bugtrackerUrl");
    roles.insert(FaqUrlRole, "faqUrl");
    roles.insert(DonationUrlRole, "donationUrl");
    roles.insert(TranslateUrlRole, "translateUrl");
    roles.insert(VersionRole, "version");
    roles.insert(InstalledVersionRole, "installedVersion");
    roles.insert(AvailableVersionRole, "availableVersion");
    roles.insert(UpdatesAvailableRole, "updatesAvailable");
    roles.insert(DownloadSizeRole, "downloadSize");
    roles.insert(InstalledSizeRole, "installedSize");
    roles.insert(SizeRole, "size");
    roles.insert(ChangeLogRole, "changeLog");
    roles.insert(InstalledRole, "installed");
    roles.insert(LocalizedRole, "localized");
    roles.insert(KudosRole, "kudos");
    roles.insert(KudosPercentageRole, "kudosPercentage");
    roles.insert(RatingRole, "rating");
    return roles;
}

int ResourcesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const ResourcesModel);
    return d->proxies.size();
}

QVariant ResourcesModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ResourcesModel);

    if (!index.isValid() || index.row() < 0 || index.row() >= rowCount())
        return QVariant();
    if (!d->manager)
        return QVariant();

    auto proxy = d->proxies.at(index.row());
    if (!proxy)
        return QVariant();
    auto *source = proxy->defaultSource();
    if (!source) {
        qCWarning(lcAppCenter, "No source found for index %d", index.row());
        return QVariant();
    }
    auto *resource = proxy->selectedResource();
    if (!resource) {
        qCWarning(lcAppCenter, "No resource found for source \"%s\"",
                  qPrintable(proxy->defaultSource()->name()));
        return QVariant();
    }

    switch (role) {
    case Qt::DisplayRole:
        return resource->name();
    case ProxyRole:
        return QVariant::fromValue<ResourceProxy *>(proxy);
    case TypeRole:
        return resource->type();
    case StateRole:
        return resource->state();
    case AppIdRole:
        return resource->appId();
    case NameRole:
        return resource->name();
    case SummaryRole:
        return resource->summary();
    case DescriptionRole:
        return resource->description();
    case IconNameRole:
        return resource->iconName();
    case IconUrlRole:
        return resource->iconUrl();
    case PackageNameRole:
        return resource->packageName();
    case ArchitectureRole:
        return resource->architecture();
    case LicenseRole:
        return resource->license();
    case CategoryRole:
        return resource->category();
    case HomePageUrlRole:
        return resource->homepageUrl();
    case BugTrackerUrlRole:
        return resource->bugtrackerUrl();
    case FaqUrlRole:
        return resource->faqUrl();
    case HelpUrlRole:
        return resource->helpUrl();
    case DonationUrlRole:
        return resource->donationUrl();
    case TranslateUrlRole:
        return resource->translateUrl();
    case VersionRole:
        return resource->version();
    case InstalledVersionRole:
        return resource->installedVersion();
    case AvailableVersionRole:
        return resource->availableVersion();
    case UpdatesAvailableRole:
        return resource->updatesAvailable();
    case DownloadSizeRole:
        return resource->downloadSize();
    case InstalledSizeRole:
        return resource->installedSize();
    case SizeRole:
        return resource->size();
    case ChangeLogRole:
        return resource->changeLog();
    case InstalledRole:
        return resource->isInstalled();
    case LocalizedRole:
        return resource->isLocalized();
    case KudosRole:
        return QVariant::fromValue(resource->kudos());
    case KudosPercentageRole:
        return resource->kudosPercentage();
    case RatingRole:
        return QVariant::fromValue<Rating *>(resource->rating());
    default:
        break;
    }

    return QVariant();
}

} // namespace AppCenter

} // namespace Liri

#include "moc_resourcesmodel.cpp"
