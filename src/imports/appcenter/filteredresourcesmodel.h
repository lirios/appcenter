// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef INSTALLEDAPPSPROXYMODEL_H
#define INSTALLEDAPPSPROXYMODEL_H

#include <QSortFilterProxyModel>

class FilteredResourcesModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Filter filter READ filter WRITE setFilter NOTIFY filterChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
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
    void countChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    Filter m_filter = InstalledApps;
};

#endif // INSTALLEDAPPSPROXYMODEL_H
