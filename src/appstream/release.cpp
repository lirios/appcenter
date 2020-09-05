// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "release.h"

namespace Liri {

namespace AppStream {

/*
 * ReleaseData
 */

class ReleaseData : public QSharedData
{
public:
    ReleaseData()
    {
    }
};

/*
 * Release
 */

Release::Release()
    : d(new ReleaseData())
{
}

} // namespace AppStream

} // namespace Liri
