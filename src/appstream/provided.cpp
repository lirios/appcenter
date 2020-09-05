// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "provided.h"

namespace Liri {

namespace AppStream {

/*
 * ProvidedData
 */

class ProvidedData : public QSharedData
{
public:
    ProvidedData()
    {
    }
};

/*
 * Provided
 */

Provided::Provided()
    : d(new ProvidedData())
{
}

} // namespace AppStream

} // namespace Liri
