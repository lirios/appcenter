// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "odrsplugin.h"

namespace Liri {

namespace AppCenter {

OdrsPlugin::OdrsPlugin(QObject *parent)
    : Liri::AppCenter::ReviewsBackendPlugin(parent)
{
}

ReviewsBackend *OdrsPlugin::create(SoftwareManager *manager)
{
    return new OdrsBackend(manager);
}

} // namespace AppCenter

} // namespace Liri
