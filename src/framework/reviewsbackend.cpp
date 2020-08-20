// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "reviewsbackend.h"

namespace Liri {

namespace AppCenter {

ReviewsBackend::ReviewsBackend(SoftwareManager *manager, QObject *parent)
    : QObject(parent)
{
    Q_UNUSED(manager);
}

ReviewsBackend::~ReviewsBackend()
{
}

void ReviewsBackend::initialize()
{
}

} // namespace AppCenter

} // namespace Liri
