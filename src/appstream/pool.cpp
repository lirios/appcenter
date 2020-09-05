// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "pool.h"

namespace Liri {

namespace AppStream {

/*
 * PoolData
 */

class PoolData : public QSharedData
{
public:
    PoolData()
    {
    }
};

/*
 * Pool
 */

Pool::Pool()
    : d(new PoolData())
{
}

} // namespace AppStream

} // namespace Liri
