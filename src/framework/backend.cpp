// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "backend.h"

namespace Liri {

namespace AppCenter {

Backend::Backend(SoftwareManager *manager, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(manager);
}

Backend::~Backend()
{
}

} // namespace AppCenter

} // namespace Liri
