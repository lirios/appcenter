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

#ifndef LIRIAPPCENTERRESOURCESMODEL_H
#define LIRIAPPCENTERRESOURCESMODEL_H

#include <QAbstractListModel>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class ResourcesModelPrivate;
class SoftwareResource;

class LIRIAPPCENTER_EXPORT ResourcesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ResourcesModel)
    Q_DISABLE_COPY(ResourcesModel)
public:
    enum Roles {
        ResourceRole = Qt::UserRole + 1,
        TypeRole,
        StateRole,
        NameRole
    };

    explicit ResourcesModel(QObject *parent = nullptr);
    ~ResourcesModel();

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void addResource(SoftwareResource *resource);
    void removeResource(SoftwareResource *resource);

private:
    ResourcesModelPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERRESOURCESMODEL_H
