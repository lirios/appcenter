// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRIAPPCENTERSOFTWAREMANAGER_P_H
#define LIRIAPPCENTERSOFTWAREMANAGER_P_H

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

#include <QLoggingCategory>

#include <LiriAppCenter/Backend>
#include <LiriAppCenter/BackendPlugin>
#include <LiriAppCenter/ResourceProxy>
#include <LiriAppCenter/ReviewsBackend>
#include <LiriAppCenter/ReviewsBackendPlugin>
#include <LiriAppCenter/SoftwareManager>

Q_DECLARE_LOGGING_CATEGORY(lcAppCenter)

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT SoftwareManagerPrivate
{
    Q_DECLARE_PUBLIC(SoftwareManager)
public:
    SoftwareManagerPrivate(SoftwareManager *self);
    ~SoftwareManagerPrivate();

    void populate();

    static SoftwareManagerPrivate *get(SoftwareManager *self) { return self->d_func(); }

    QList<Backend *> resourcesBackends;
    QList<ReviewsBackend *> reviewsBackends;

    SourcesModel *sourcesModel = nullptr;
    uint updatesCount = 0;

    QList<ResourceProxy *> proxies;
    QHash<QString, ResourceProxy *> proxiesMap;
    QList<SoftwareResource *> resources;

protected:
    SoftwareManager *q_ptr;
};

} // namespace AppCenter

} // namesapce Liri

#endif // LIRIAPPCENTERSOFTWAREMANAGER_P_H
