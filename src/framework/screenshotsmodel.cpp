// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
    connect(this, &QAbstractListModel::rowsInserted,
            this, &ScreenshotsModel::countChanged);
    connect(this, &QAbstractListModel::rowsRemoved,
            this, &ScreenshotsModel::countChanged);
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
