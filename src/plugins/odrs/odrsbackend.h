// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_ODRS_ODRSBACKEND_H
#define LIRI_APPCENTER_ODRS_ODRSBACKEND_H

#include <QLoggingCategory>

#include <LiriAppCenter/ReviewsBackend>
#include <LiriAppCenter/private/cachednetworkaccessmanager_p.h>

Q_DECLARE_LOGGING_CATEGORY(lcOdrsBackend)

class QNetworkAccessManager;
class QNetworkReply;

namespace Liri {

namespace AppCenter {

class OdrsBackend : public ReviewsBackend
{
    Q_OBJECT
public:
    enum ReviewAction {
        SubmitReview = 0,
        ReportReview,
        UpVoteReview,
        DownVoteReview,
        DismissReview,
        RemoveReview
    };
    Q_ENUM(ReviewAction)

    explicit OdrsBackend(SoftwareManager *manager,
                         QObject *parent = nullptr);

    void fetchRatings() override;
    void fetchReviews(SoftwareResource *resource) override;

    QList<Rating *> ratings() const override;
    Rating *ratingForSofwareResource(SoftwareResource *resource) const override;

    QList<Review *> reviews() const override;

    bool submitReview(SoftwareResource *resource,
                      const QString &summary,
                      const QString &description,
                      int rating) override;
    void reportReview(Review *review) override;
    void upVoteReview(Review *review) override;
    void downVoteReview(Review *review) override;
    void dismissReview(Review *review) override;
    void removeReview(Review *review) override;

private:
    CachedNetworkAccessManager *m_manager = nullptr;
    QHash<QString, Rating *> m_ratings;
    QList<Review *> m_reviews;

    void parseRatings(const QString &fileName);
    void postReview(ReviewAction action, Review *review);
    void parseReviews(const QJsonDocument &json, SoftwareResource *resource);
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_ODRS_ODRSBACKEND_H
