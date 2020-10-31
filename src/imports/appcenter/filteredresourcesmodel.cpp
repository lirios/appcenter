// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDateTime>

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/SoftwareResource>

#include "filteredresourcesmodel.h"

using namespace Liri::AppCenter;

FilteredResourcesModel::FilteredResourcesModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
    connect(this, &QSortFilterProxyModel::rowsInserted,
            this, &FilteredResourcesModel::countChanged);
    connect(this, &QSortFilterProxyModel::rowsRemoved,
            this, &FilteredResourcesModel::countChanged);
    connect(this, &QSortFilterProxyModel::dataChanged,
            this, &FilteredResourcesModel::countChanged);

    auto *model = new ResourcesModel(this);
    connect(model, &ResourcesModel::managerChanged,
            this, &FilteredResourcesModel::managerChanged);

    setDynamicSortFilter(true);
    setSortRole(Liri::AppCenter::ResourcesModel::NameRole);
    setSortLocaleAware(true);
    setSourceModel(model);
    sort(0);
}

SoftwareManager *FilteredResourcesModel::manager() const
{
    return qobject_cast<ResourcesModel *>(sourceModel())->manager();
}

void FilteredResourcesModel::setManager(SoftwareManager *manager)
{
    qobject_cast<ResourcesModel *>(sourceModel())->setManager(manager);
}

FilteredResourcesModel::Filter FilteredResourcesModel::filter() const
{
    return m_filter;
}

void FilteredResourcesModel::setFilter(FilteredResourcesModel::Filter filter)
{
    if (m_filter == filter)
        return;

    m_filter = filter;
    invalidateFilter();
    Q_EMIT modelFilterChanged();
}

bool FilteredResourcesModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    auto type = sourceModel()->data(index, ResourcesModel::TypeRole).value<SoftwareResource::Type>();
    bool isInstalled  = sourceModel()->data(index, ResourcesModel::ProxyInstalledRole).toBool();
    bool hasUpdatesAvailable  = sourceModel()->data(index, ResourcesModel::ProxyUpdatesAvailableRole).toBool();

    switch (m_filter) {
    case AllApps:
        return type == SoftwareResource::App;
    case NotInstalledApps:
        return type == SoftwareResource::App && !isInstalled;
    case InstalledApps:
        return type == SoftwareResource::App && isInstalled;
    case Updates:
        return type == SoftwareResource::App && hasUpdatesAvailable;
    }

    return false;
}

bool FilteredResourcesModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QVariant left = sourceModel()->data(source_left, sortRole());
    QVariant right = sourceModel()->data(source_right, sortRole());
    int type = left.userType();

    switch (type) {
    case QMetaType::QString:
        return QString::localeAwareCompare(left.toString(), right.toString()) < 0;
    case QMetaType::Int:
    case QMetaType::Short:
        return left.toInt() < right.toInt();
    case QMetaType::UInt:
    case QMetaType::UShort:
        return left.toUInt() < right.toUInt();
    case QMetaType::Long:
    case QMetaType::LongLong:
        return left.toLongLong() < right.toLongLong();
    case QMetaType::ULong:
    case QMetaType::ULongLong:
        return left.toULongLong() < right.toULongLong();
    case QMetaType::Float:
        return left.toFloat() < right.toFloat();
    case QMetaType::Double:
        return left.toDouble() < right.toDouble();
    case QMetaType::QDate:
        return left.toDate() < right.toDate();
    case QMetaType::QDateTime:
        return left.toDateTime() < right.toDateTime();
    default:
        break;
    }

    return false;
}
