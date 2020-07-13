// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "softwareresource.h"
#include "rating.h"
#include "resourcesmodel.h"
#include "resourcesmodel_p.h"

namespace Liri {

namespace AppCenter {

ResourcesModelPrivate::ResourcesModelPrivate()
{
}

ResourcesModel::ResourcesModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ResourcesModelPrivate())
{
}

ResourcesModel::~ResourcesModel()
{
    delete d_ptr;
}

QHash<int, QByteArray> ResourcesModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ResourceRole, QByteArrayLiteral("resource"));
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
    roles.insert(OriginRole, "origin");
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
    return d->resources.size();
}

QVariant ResourcesModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ResourcesModel);

    if (!index.isValid() || index.row() < 0 || index.row() >= d->resources.size())
        return QVariant();

    SoftwareResource *resource = d->resources.at(index.row());

    switch (role) {
    case Qt::DisplayRole:
        return resource->name();
    case ResourceRole:
        return QVariant::fromValue<SoftwareResource *>(resource);
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
    case OriginRole:
        return resource->origin();
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

void ResourcesModel::addResource(SoftwareResource *resource)
{
    Q_D(ResourcesModel);

    beginInsertRows(QModelIndex(), d->resources.size(), d->resources.size() + 1);
    d->resources.append(resource);
    endInsertRows();

    QMetaObject::Connection connection =
            connect(resource, &SoftwareResource::stateChanged, this, [this, d, resource] {
        int row = d->resources.indexOf(resource);
        if (row < 0 || row > d->resources.size())
            return;
        QModelIndex modelIndex = index(row);
        Q_EMIT dataChanged(modelIndex, modelIndex);
    });
    d->stateChangedConnections.append(connection);
}

void ResourcesModel::removeResource(SoftwareResource *resource)
{
    Q_D(ResourcesModel);

    int index = d->resources.indexOf(resource);

    QMetaObject::Connection connection = d->stateChangedConnections.at(index);
    disconnect(connection);
    d->stateChangedConnections.remove(index);

    beginRemoveRows(QModelIndex(), index, index);
    d->resources.remove(index);
    endRemoveRows();
}

} // namespace AppCenter

} // namespace Liri
