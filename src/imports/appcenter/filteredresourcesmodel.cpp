// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/SoftwareResource>

#include "filteredresourcesmodel.h"

using namespace Liri::AppCenter;

FilteredResourcesModel::FilteredResourcesModel(QObject *parent)
    : QSortFilterProxyModel(parent)
{
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
    Q_EMIT filterChanged();
}

bool FilteredResourcesModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    QModelIndex index = sourceModel()->index(source_row, 0, source_parent);
    auto type = sourceModel()->data(index, ResourcesModel::TypeRole).value<SoftwareResource::Type>();
    auto state  = sourceModel()->data(index, ResourcesModel::StateRole).value<SoftwareResource::State>();

    switch (m_filter) {
    case AllApps:
        return type == SoftwareResource::App;
    case NotInstalledApps:
        return type == SoftwareResource::App &&
                state == SoftwareResource::NotInstalledState;
    case InstalledApps:
        return type == SoftwareResource::App &&
                (state == SoftwareResource::InstalledState ||
                 state == SoftwareResource::UpgradableState);
    case Updates:
        return type == SoftwareResource::App &&
                state == SoftwareResource::UpgradableState;
    }

    return false;
}

bool FilteredResourcesModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    QString nameLeft = sourceModel()->data(source_left, ResourcesModel::NameRole).toString();
    QString nameRight = sourceModel()->data(source_right, ResourcesModel::NameRole).toString();
    return nameLeft < nameRight;
}
