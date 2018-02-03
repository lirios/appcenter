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

#include "softwareresource.h"
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
    roles.insert(NameRole, QByteArrayLiteral("name"));
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
    case NameRole:
        return resource->name();
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
