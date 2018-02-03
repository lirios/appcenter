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

#ifndef LIRIAPPCENTERSCREENSHOTSMODEL_H
#define LIRIAPPCENTERSCREENSHOTSMODEL_H

#include <QAbstractListModel>
#include <QUrl>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class ScreenshotsModelPrivate;

class LIRIAPPCENTER_EXPORT ScreenshotsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(SoftwareResource *app READ app WRITE setApp NOTIFY appChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
    Q_DECLARE_PRIVATE(ScreenshotsModel)
    Q_DISABLE_COPY(ScreenshotsModel)
public:
    enum Roles {
        ThumbnailUrlRole = Qt::UserRole + 1,
        ThumbnailSizeRole,
        ScreenshotUrlRole,
        ScreenshotSizeRole
    };

    explicit ScreenshotsModel(QObject *parent = nullptr);
    ~ScreenshotsModel();

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    SoftwareResource *app() const;
    void setApp(SoftwareResource *app);

    Q_INVOKABLE QUrl thumbnailUrlAt(int index) const;
    Q_INVOKABLE QUrl screenshotUrlAt(int index) const;

Q_SIGNALS:
    void appChanged();
    void countChanged();

private:
    ScreenshotsModelPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSCREENSHOTSMODEL_H
