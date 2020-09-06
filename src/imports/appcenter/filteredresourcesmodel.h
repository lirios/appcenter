// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef INSTALLEDAPPSPROXYMODEL_H
#define INSTALLEDAPPSPROXYMODEL_H

#include <QSortFilterProxyModel>

#include <LiriAppCenter/SoftwareManager>

class FilteredResourcesModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(Liri::AppCenter::SoftwareManager *manager READ manager WRITE setManager NOTIFY managerChanged)
    Q_PROPERTY(Filter filter READ filter WRITE setFilter NOTIFY modelFilterChanged)
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

    Liri::AppCenter::SoftwareManager *manager() const;
    void setManager(Liri::AppCenter::SoftwareManager *manager);

    Filter filter() const;
    void setFilter(Filter filter);

Q_SIGNALS:
    void managerChanged();
    void modelFilterChanged();
    void countChanged();

protected:
    bool filterAcceptsRow(int source_row, const QModelIndex &source_parent) const override;
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    Filter m_filter = InstalledApps;
};

#endif // INSTALLEDAPPSPROXYMODEL_H
