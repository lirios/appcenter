// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEWSBACKEND_H
#define LIRI_APPCENTER_REVIEWSBACKEND_H

#include <LiriAppCenter/SoftwareManager>

namespace Liri {

namespace AppCenter {

class Rating;
class Review;
class SoftwareManager;

class LIRIAPPCENTER_EXPORT ReviewsBackend : public QObject
{
    Q_OBJECT
public:
    explicit ReviewsBackend(SoftwareManager *manager,
                            QObject *parent = nullptr);
    virtual ~ReviewsBackend();

    virtual void initialize();

    virtual void fetchRatings() = 0;
    virtual void fetchReviews(SoftwareResource *resource) = 0;

    virtual QList<Rating *> ratings() const = 0;
    virtual Rating *ratingForSofwareResource(SoftwareResource *resource) const = 0;

    virtual QList<Review *> reviews() const = 0;

    virtual void submitReview(Review *review) = 0;
    virtual void reportReview(Review *review) = 0;
    virtual void upvoteReview(Review *review) = 0;
    virtual void downvoteReview(Review *review) = 0;
    virtual void dismissReview(Review *review) = 0;
    virtual void removeReview(Review *review) = 0;

Q_SIGNALS:
    void ratingsFetched();
    void ratingsReady();
    void reviewAdded(Review *review);
    void reviewSubmitted(Review *review);
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_REVIEWSBACKEND_H
