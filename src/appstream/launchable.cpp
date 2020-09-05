// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "launchable.h"

namespace Liri {

namespace AppStream {

/*
 * LaunchableData
 */

class LaunchableData : public QSharedData
{
public:
    LaunchableData()
    {
    }
};

/*
 * Launchable
 */

Launchable::Launchable()
    : d(new LaunchableData())
{
}

} // namespace AppStream

} // namespace Liri
