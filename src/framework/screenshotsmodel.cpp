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

#include "screenshotsmodel.h"
#include "screenshotsmodel_p.h"

namespace Liri {

namespace AppCenter {

ScreenshotsModelPrivate::ScreenshotsModelPrivate(ScreenshotsModel *self)
    : q_ptr(self)
{
}

ScreenshotsModel::ScreenshotsModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ScreenshotsModelPrivate(this))
{
}

ScreenshotsModel::~ScreenshotsModel()
{
    delete d_ptr;
}

QHash<int, QByteArray> ScreenshotsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractListModel::roleNames();
    roles.insert(ThumbnailUrlRole, QByteArrayLiteral("thumbnailUrl"));
    roles.insert(ThumbnailSizeRole, QByteArrayLiteral("thumbnailSize"));
    roles.insert(ScreenshotUrlRole, QByteArrayLiteral("screenshotUrl"));
    roles.insert(ScreenshotSizeRole, QByteArrayLiteral("screenshotSize"));
    return roles;
}

int ScreenshotsModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    Q_D(const ScreenshotsModel);
    return d->thumbnails.size();
}

QVariant ScreenshotsModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ScreenshotsModel);

    if (!index.isValid() || index.row() < 0 || index.row() >= d->thumbnails.size())
        return QVariant();

    switch (role) {
    case ThumbnailUrlRole:
        return d->thumbnails.at(index.row()).url();
    case ThumbnailSizeRole:
        return d->thumbnails.at(index.row()).size();
    case ScreenshotUrlRole:
        return d->screenshots.at(index.row()).url();
    case ScreenshotSizeRole:
        return d->screenshots.at(index.row()).size();
    default:
        break;
    }

    return QVariant();
}

SoftwareResource *ScreenshotsModel::app() const
{
    Q_D(const ScreenshotsModel);
    return d->app;
}

void ScreenshotsModel::setApp(SoftwareResource *app)
{
    Q_D(ScreenshotsModel);

    if (d->app == app)
        return;

    beginResetModel();
    d->app = app;
    d->thumbnails = d->app->thumbnails();
    d->screenshots = d->app->screenshots();
    Q_EMIT appChanged();
    endResetModel();
}

QUrl ScreenshotsModel::thumbnailUrlAt(int index) const
{
    Q_D(const ScreenshotsModel);

    if (index < d->thumbnails.size())
        return d->thumbnails.at(index).url();
    return QUrl();
}

QUrl ScreenshotsModel::screenshotUrlAt(int index) const
{
    Q_D(const ScreenshotsModel);

    if (index < d->screenshots.size())
        return d->screenshots.at(index).url();
    return QUrl();
}

} // namespace AppCenter

} // namespace Liri
