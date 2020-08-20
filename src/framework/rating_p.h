// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_RATING_P_H
#define LIRI_APPCENTER_RATING_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Liri API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include <LiriAppCenter/Rating>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT RatingPrivate
{
public:
    RatingPrivate() = default;

    QString appId;
    quint64 numVotes = 0;
    qreal rating = 0.0;
    quint64 star0 = 0;
    quint64 star1 = 0;
    quint64 star2 = 0;
    quint64 star3 = 0;
    quint64 star4 = 0;
    quint64 star5 = 0;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_RATING_P_H
