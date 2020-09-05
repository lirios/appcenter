// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPSTREAM_POOL_H
#define LIRI_APPSTREAM_POOL_H

#include <QSharedDataPointer>

namespace Liri {

namespace AppStream {

class PoolData;

class Pool
{
public:
    Pool();

private:
    QSharedDataPointer<PoolData> d;
};

} // namespace AppStream

} // namespace Liri

#endif // LIRI_APPSTREAM_POOL_H
