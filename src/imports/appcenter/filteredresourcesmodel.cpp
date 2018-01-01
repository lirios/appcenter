/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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
