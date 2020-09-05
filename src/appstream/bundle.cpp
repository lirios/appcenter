// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "bundle.h"

namespace Liri {

namespace AppStream {

/*
 * BundleData
 */

class BundleData : public QSharedData
{
public:
    BundleData()
    {
    }
};

/*
 * Bundle
 */

Bundle::Bundle()
    : d(new BundleData())
{
}

} // namespace AppStream

} // namespace Liri
