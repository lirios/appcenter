// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "icon.h"

namespace Liri {

namespace AppStream {

/*
 * IconData
 */

class IconData : public QSharedData
{
public:
    IconData()
    {
    }
};

/*
 * Icon
 */

Icon::Icon()
    : d(new IconData())
{
}

} // namespace AppStream

} // namespace Liri
