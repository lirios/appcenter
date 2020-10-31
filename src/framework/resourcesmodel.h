// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERRESOURCESMODEL_H
#define LIRIAPPCENTERRESOURCESMODEL_H

#include <QAbstractListModel>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class ResourcesModelPrivate;
class SoftwareManager;
class SoftwareResource;

class LIRIAPPCENTER_EXPORT ResourcesModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ResourcesModel)
    Q_DISABLE_COPY(ResourcesModel)
    Q_PROPERTY(SoftwareManager *manager READ manager WRITE setManager NOTIFY managerChanged)
public:
    enum Roles {
        ProxyRole = Qt::UserRole + 1,
        TypeRole,
        ProxyStateRole,
        ProxyUpdatesAvailableRole,
        ProxyInstalledRole,
        StateRole,
        AppIdRole,
        NameRole,
        SummaryRole,
        DescriptionRole,
        IconNameRole,
        IconUrlRole,
        PackageNameRole,
        ArchitectureRole,
        LicenseRole,
        CategoryRole,
        HomePageUrlRole,
        BugTrackerUrlRole,
        FaqUrlRole,
        HelpUrlRole,
        DonationUrlRole,
        TranslateUrlRole,
        VersionRole,
        InstalledVersionRole,
        AvailableVersionRole,
        UpdatesAvailableRole,
        DownloadSizeRole,
        InstalledSizeRole,
        SizeRole,
        ChangeLogRole,
        InstalledRole,
        LocalizedRole,
        KudosRole,
        KudosPercentageRole,
        RatingRole
    };

    explicit ResourcesModel(QObject *parent = nullptr);
    ~ResourcesModel();

    SoftwareManager *manager() const;
    void setManager(SoftwareManager *manager);

    QHash<int,QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

Q_SIGNALS:
    void managerChanged();

private:
    ResourcesModelPrivate *const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void handlePopulated(QList<ResourceProxy*>))
    Q_PRIVATE_SLOT(d_func(), void handleProxyRemoved(ResourceProxy*))
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERRESOURCESMODEL_H
