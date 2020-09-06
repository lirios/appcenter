// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERRESOURCESMODEL_P_H
#define LIRIAPPCENTERRESOURCESMODEL_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <QHash>

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/ResourceProxy>
#include <LiriAppCenter/SoftwareManager>

namespace Liri {

namespace AppCenter {

class ResourcesModelPrivate
{
    Q_DECLARE_PUBLIC(ResourcesModel)
public:
    explicit ResourcesModelPrivate(ResourcesModel *self);

    void addProxies(QList<ResourceProxy *> list);

    void handlePopulated(QList<ResourceProxy *> list);
    void handleProxyAdded(ResourceProxy *proxy);
    void handleProxyRemoved(ResourceProxy *proxy);

    bool initialized = false;
    SoftwareManager *manager = nullptr;
    QHash<ResourceProxy *, QMetaObject::Connection> connections;

protected:
    ResourcesModel *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERRESOURCESMODEL_P_H
