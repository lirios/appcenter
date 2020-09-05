// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QSharedData>

#include "contentrating.h"

namespace Liri {

namespace AppStream {

/*
 * ContentRatingData
 */

class ContentRatingData : public QSharedData
{
public:
    ContentRatingData()
    {
    }
};

/*
 * ContentRating
 */

ContentRating::ContentRating()
    : d(new ContentRatingData())
{
}

} // namespace AppStream

} // namespace Liri
