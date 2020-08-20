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

private:
    ResourcesModelPrivate *const d_ptr;

    void addResource(SoftwareResource *resource);
    void removeResource(SoftwareResource *resource);

    friend class SoftwareManager;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERRESOURCESMODEL_H
