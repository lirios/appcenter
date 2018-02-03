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

#ifndef INSTALLEDAPPSPROXYMODEL_H
#define INSTALLEDAPPSPROXYMODEL_H

#include <QSortFilterProxyModel>

class FilteredResourcesModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Filter filter READ filter WRITE setFilter NOTIFY filterChanged)
public:
    enum Filter {
        AllApps,
        NotInstalledApps,
        InstalledApps,
        Updates
    };
    Q_ENUM(Filter);

    explicit FilteredResourcesModel(QObject *parent = nullptr);

    Filter filter() const;
    void setFilter(Filter filter);

Q_SIGNALS:
    void filterChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    Filter m_filter = InstalledApps;
};

#endif // INSTALLEDAPPSPROXYMODEL_H
