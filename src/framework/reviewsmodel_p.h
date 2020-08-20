// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEWSMODEL_P_H
#define LIRI_APPCENTER_REVIEWSMODEL_P_H

#include <LiriAppCenter/ReviewsBackend>
#include <LiriAppCenter/ReviewsModel>

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

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT ReviewsModelPrivate
{
    Q_DECLARE_PUBLIC(ReviewsModel)
public:
    explicit ReviewsModelPrivate(ReviewsModel *self);

    void handleReviewsFetched();

    SoftwareResource *resource = nullptr;
    QList<Review *> reviews;

protected:
    ReviewsModel *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_REVIEWSMODEL_P_H
