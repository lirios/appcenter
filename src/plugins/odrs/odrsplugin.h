// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_ODRS_ODRSPLUGIN_H
#define LIRI_APPCENTER_ODRS_ODRSPLUGIN_H

#include <LiriAppCenter/ReviewsBackendPlugin>

#include "odrsbackend.h"

namespace Liri {

namespace AppCenter {

class OdrsPlugin : public Liri::AppCenter::ReviewsBackendPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID ReviewsBackendPlugin_iid FILE "odrs.json")
    Q_INTERFACES(Liri::AppCenter::ReviewsBackendPlugin)
public:
    explicit OdrsPlugin(QObject *parent = nullptr);

    ReviewsBackend *create(SoftwareManager *manager) override;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_ODRS_ODRSPLUGIN_H
