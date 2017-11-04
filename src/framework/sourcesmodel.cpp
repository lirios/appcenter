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

#include <LiriAppCenter/SoftwareSource>

#include "sourcesmodel.h"
#include "sourcesmodel_p.h"

namespace Liri {

namespace AppCenter {

SourcesModelPrivate::SourcesModelPrivate()
{
}

SourcesModelPrivate::~SourcesModelPrivate()
{
    qDeleteAll(sources);
}

SourcesModel::SourcesModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new SourcesModelPrivate())
{
    Q_D(SourcesModel);

    d->roles = QAbstractListModel::roleNames();
    d->roles.insert(SourceRole, QByteArrayLiteral("source"));
    d->roles.insert(SectionRole, QByteArrayLiteral("section"));
    d->roles.insert(NameRole, QByteArrayLiteral("name"));
    d->roles.insert(TitleRole, QByteArrayLiteral("title"));
    d->roles.insert(EnabledRole, QByteArrayLiteral("enabled"));
    d->roles.insert(UrlRole, QByteArrayLiteral("url"));
    d->roles.insert(PriorityRole, QByteArrayLiteral("priority"));
}

SourcesModel::~SourcesModel()
{
    delete d_ptr;
}

QHash<int, QByteArray> SourcesModel::roleNames() const
{
    Q_D(const SourcesModel);
    return d->roles;
}

int SourcesModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const SourcesModel);
    return d->sources.size();
}

QVariant SourcesModel::data(const QModelIndex &index, int role) const
{
    Q_D(const SourcesModel);

    if (!index.isValid() || index.row() < 0 || index.row() >= d->sources.size())
        return QVariant();

    SoftwareSource *source = d->sources.at(index.row());

    switch (role) {
    case SourceRole:
        return QVariant::fromValue<SoftwareSource *>(source);
    case SectionRole:
        return source->section();
    case NameRole:
        return source->name();
    case TitleRole:
        return source->title();
    case EnabledRole:
        return source->isEnabled();
    case UrlRole:
        return source->url();
    case PriorityRole:
        return source->priority();
    default:
        break;
    }

    return QVariant();
}

void SourcesModel::setItemRoleNames(const QHash<int, QByteArray> &roles)
{
    Q_D(SourcesModel);
    d->roles = roles;
}

void SourcesModel::addSource(SoftwareSource *source)
{
    Q_D(SourcesModel);

    if (d->sources.contains(source))
        return;

    int row = d->sources.size();
    beginInsertRows(QModelIndex(), row, row);
    d->sources.append(source);
    endInsertRows();
}

void SourcesModel::removeSource(SoftwareSource *source)
{
    Q_D(SourcesModel);

    int row = d->sources.indexOf(source);
    if (row >= 0) {
        beginRemoveRows(QModelIndex(), row, row);
        d->sources.remove(row);
        endRemoveRows();
    }
}

SoftwareSource *SourcesModel::findSource(const QString &name) const
{
    Q_D(const SourcesModel);

    for (auto source : qAsConst(d->sources)) {
        if (source->name() == name)
            return source;
    }

    return nullptr;
}

} // namespace AppCenter

} // namespace Liri
