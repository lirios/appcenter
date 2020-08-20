// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEWSBACKENDPLUGIN_H
#define LIRI_APPCENTER_REVIEWSBACKENDPLUGIN_H

#include <QObject>

#include <LiriAppCenter/ReviewsBackend>
#include <LiriAppCenter/SoftwareManager>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT ReviewsBackendPlugin : public QObject
{
    Q_OBJECT
public:
    explicit ReviewsBackendPlugin(QObject *parent = nullptr);

    virtual ReviewsBackend *create(SoftwareManager *manager) = 0;
};

} // namespace AppCenter

} // namespace Liri

#define ReviewsBackendPlugin_iid "io.liri.AppCenter.ReviewsBackendPlugin"

Q_DECLARE_INTERFACE(Liri::AppCenter::ReviewsBackendPlugin, ReviewsBackendPlugin_iid)

#endif // LIRI_APPCENTER_REVIEWSBACKENDPLUGIN_H
