// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "suggested.h"

namespace Liri {

namespace AppStream {

/*
 * SuggestedData
 */

class SuggestedData : public QSharedData
{
public:
    SuggestedData()
    {
    }
};

/*
 * Suggested
 */

Suggested::Suggested()
    : d(new SuggestedData())
{
}

} // namespace AppStream

} // namespace Liri
