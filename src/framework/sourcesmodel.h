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

#ifndef LIRIAPPCENTERSOURCESMODEL_H
#define LIRIAPPCENTERSOURCESMODEL_H

#include <QAbstractListModel>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class SoftwareSource;
class SourcesModelPrivate;

class LIRIAPPCENTER_EXPORT SourcesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(SourcesModel)
    Q_DISABLE_COPY(SourcesModel)
public:
    enum Roles {
        SourceRole = Qt::UserRole + 1,
        SectionRole,
        NameRole,
        TitleRole,
        CommentRole,
        DescriptionRole,
        EnabledRole,
        GpgVerifyRole,
        UrlRole,
        IconUrlRole,
        PriorityRole
    };

    explicit SourcesModel(QObject *parent = nullptr);
    ~SourcesModel();

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void setItemRoleNames(const QHash<int,QByteArray> &roles);

    void addSource(SoftwareSource *source);
    void removeSource(SoftwareSource *source);

    SoftwareSource *findSource(const QString &name) const;

private:
    SourcesModelPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERSOURCESMODEL_H
