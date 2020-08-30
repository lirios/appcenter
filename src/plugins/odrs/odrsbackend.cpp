// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDateTime>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QtMath>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QStandardPaths>

#include <LiriAppCenter/Rating>
#include <LiriAppCenter/Review>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/private/review_p.h>
#include <LiriAppCenter/private/softwareresource_p.h>
#include <LiriLocalDevice/OsRelease>

#include "odrsbackend.h"
#include "odrsutils.h"

#define USER_AGENT QStringLiteral("liri-appcenter/1.0")
#define ODRS_URL "https://odrs.gnome.org/1.0/reviews/api"
#define CACHE_TIME_SECS 60 * 60 * 24

Q_LOGGING_CATEGORY(lcOdrsBackend, "liri.appcenter.odrs")

namespace Liri {

namespace AppCenter {

OdrsBackend::OdrsBackend(SoftwareManager *manager, QObject *parent)
    : ReviewsBackend(manager, parent)
    , m_manager(new CachedNetworkAccessManager(QStringLiteral("odrs"), this))
{
    m_manager->setAutoDeleteReplies(true);
}

void OdrsBackend::fetchRatings()
{
    const QString cacheFileName =
            QStandardPaths::writableLocation(QStandardPaths::CacheLocation) +
            QStringLiteral("/odrs/ratings.json");
    bool fetch = false;
    QDateTime lastModification;

    // Download the ratings again only if the cache doesn't exist
    // or it's too old
    if (QFile::exists(cacheFileName)) {
        QFileInfo fileInfo(cacheFileName);
        if (fileInfo.lastModified().secsTo(QDateTime::currentDateTime()) > CACHE_TIME_SECS) {
            lastModification = fileInfo.lastModified();
            fetch = true;
        }
    } else {
        fetch = true;
    }

    if (fetch) {
        // Make sure the directory is created
        QDir cacheDir(QStandardPaths::writableLocation(QStandardPaths::CacheLocation));
        cacheDir.mkdir(QStringLiteral("odrs"));

        QNetworkRequest request;
        request.setUrl(QUrl(QStringLiteral(ODRS_URL "/ratings")));
        request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
        if (lastModification.isValid())
            request.setHeader(QNetworkRequest::LastModifiedHeader, lastModification);

        auto *reply = m_manager->get(request);
        connect(reply, &QNetworkReply::sslErrors, this,
                [](const QList<QSslError> &errors) {
            qCWarning(lcOdrsBackend, "Failed to fetch ratings:");
            for (const auto &error : errors)
                qCWarning(lcOdrsBackend, "\t%s", qPrintable(error.errorString()));
        });
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        connect(reply, &QNetworkReply::errorOccurred, this,
                [](QNetworkReply::NetworkError error) {
            if (error != QNetworkReply::NoError)
                qCWarning(lcOdrsBackend, "Failed to fetch ratings: %s",
                          qPrintable(reply->errorString()));
        });
#endif
        connect(reply, &QNetworkReply::finished, this, [this, cacheFileName, reply] {
            QFile file(cacheFileName);
            if (file.open(QFile::WriteOnly)) {
                while (!reply->atEnd())
                    file.write(reply->read(4196));
                file.close();

                Q_EMIT ratingsFetched();

                parseRatings(cacheFileName);
            } else {
                qCWarning(lcOdrsBackend, "Failed to open to \"%s\": %s",
                          qPrintable(cacheFileName),
                          qPrintable(file.errorString()));
            }
        });
    } else {
        parseRatings(cacheFileName);
    }
}

void OdrsBackend::fetchReviews(SoftwareResource *resource)
{
    auto *osRelease = new Liri::OsRelease(this);

    const QJsonObject object = {
        { QStringLiteral("user_hash"), getUserHash() },
        { QStringLiteral("app_id"), resource->appId() },
        { QStringLiteral("locale"), QLocale::system().name() },
        { QStringLiteral("distro"), osRelease->name() },
        { QStringLiteral("version"), resource->version() },
        { QStringLiteral("limit"), -1 },
    };
    const QJsonDocument document(object);
    const auto data = document.toJson(QJsonDocument::Compact);

    osRelease->deleteLater();

    QNetworkRequest request;
    request.setUrl(QUrl(QStringLiteral(ODRS_URL "/fetch")));
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=utf-8"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    request.setOriginatingObject(resource);

    auto *reply = m_manager->post(request, data);
    connect(reply, &QNetworkReply::sslErrors, this,
            [resource](const QList<QSslError> &errors) {
        qCWarning(lcOdrsBackend, "Failed to fetch reviews for \"%s\":",
                  qPrintable(resource->appId()));
        for (const auto &error : errors)
            qCWarning(lcOdrsBackend, "\t%s", qPrintable(error.errorString()));
    });
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        connect(reply, &QNetworkReply::errorOccurred, this,
                [](QNetworkReply::NetworkError error) {
            if (error != QNetworkReply::NoError)
                qCWarning(lcOdrsBackend, "Failed to fetch reviews: %s",
                          qPrintable(reply->errorString()));
        });
#endif
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        auto *resource = qobject_cast<SoftwareResource *>(reply->request().originatingObject());

        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(lcOdrsBackend, "Failed to fetch reviews for \"%s\": %s",
                      qPrintable(resource->appId()),
                      qPrintable(reply->errorString()));
            return;
        }

        QJsonParseError parseError;
        const auto json = QJsonDocument::fromJson(reply->readAll(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qCWarning(lcOdrsBackend, "Failed to fetch reviews for \"%s\": %s",
                      qPrintable(resource->appId()),
                      qPrintable(parseError.errorString()));
            return;
        }

        parseReviews(json, resource);
    });
}

QList<Rating *> OdrsBackend::ratings() const
{
    return m_ratings.values();
}

Rating *OdrsBackend::ratingForSofwareResource(SoftwareResource *resource) const
{
    if (resource->appId().isEmpty())
        return nullptr;
    return m_ratings[resource->appId()];
}

QList<Review *> OdrsBackend::reviews() const
{
    return m_reviews;
}

void OdrsBackend::submitReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/submit")), review);
}

void OdrsBackend::reportReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/report")), review);
}

void OdrsBackend::upvoteReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/upvote")), review);
}

void OdrsBackend::downvoteReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/downvote")), review);
}

void OdrsBackend::dismissReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/dismiss")), review);
}

void OdrsBackend::removeReview(Review *review)
{
    postReview(QUrl(QStringLiteral(ODRS_URL "/remove")), review);
}

void OdrsBackend::parseRatings(const QString &fileName)
{
    QFile file(fileName);
    if (file.open(QFile::ReadOnly)) {
        QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
        const QJsonObject object  = doc.object();

        m_ratings.clear();
        m_ratings.reserve(object.size());

        for (auto it = object.begin(); it != object.end(); it++) {
            QJsonObject appObject = it.value().toObject();

            const int total = appObject.value(QLatin1String("total")).toInt();
            const quint64 numVotes = qMax<quint64>(0, total);
            const QVariantMap data = {
                { QStringLiteral("star0"), appObject.value(QLatin1String("star0")).toInt() },
                { QStringLiteral("star1"), appObject.value(QLatin1String("star1")).toInt() },
                { QStringLiteral("star2"), appObject.value(QLatin1String("star2")).toInt() },
                { QStringLiteral("star3"), appObject.value(QLatin1String("star3")).toInt() },
                { QStringLiteral("star4"), appObject.value(QLatin1String("star4")).toInt() },
                { QStringLiteral("star5"), appObject.value(QLatin1String("star5")).toInt() }
            };

            auto *rating = new Rating(it.key(), numVotes, data, this);
            m_ratings[it.key()] = rating;
        }

        file.close();

        Q_EMIT ratingsReady();
    }
}

void OdrsBackend::postReview(const QUrl &url, Review *review)
{
    auto *osRelease = new Liri::OsRelease(this);

    const QJsonObject object = {
        { QStringLiteral("user_hash"), review->reviewerId() },
        { QStringLiteral("user_display"), review->reviewerName() },
        { QStringLiteral("user_skey"), review->getMetadataValue(QStringLiteral("ODRS::user_skey")).toString() },
        { QStringLiteral("app_id"), review->resource()->appId() },
        { QStringLiteral("locale"), QLocale::system().name() },
        { QStringLiteral("distro"), osRelease->name() },
        { QStringLiteral("summary"), review->summary() },
        { QStringLiteral("description"), review->description() },
        { QStringLiteral("version"), review->version() },
        { QStringLiteral("rating"), review->rating() },
        { QStringLiteral("review_id"), review->id() },
    };
    const QJsonDocument document(object);
    const auto data = document.toJson(QJsonDocument::Compact);

    osRelease->deleteLater();

    review->addMetadata(QStringLiteral("ODRS::json"), document.toVariant());

    QNetworkRequest request;
    request.setUrl(url);
    request.setHeader(QNetworkRequest::UserAgentHeader, USER_AGENT);
    request.setHeader(QNetworkRequest::ContentTypeHeader, QStringLiteral("application/json; charset=utf-8"));
    request.setHeader(QNetworkRequest::ContentLengthHeader, data.size());
    request.setOriginatingObject(review);

    auto *reply = m_manager->post(request, data);
    connect(reply, &QNetworkReply::sslErrors, this,
            [review](const QList<QSslError> &errors) {
        qCWarning(lcOdrsBackend, "Failed to post review for \"%s\":",
                  qPrintable(review->resource()->appId()));
        for (const auto &error : errors)
            qCWarning(lcOdrsBackend, "\t%s", qPrintable(error.errorString()));
    });
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        connect(reply, &QNetworkReply::errorOccurred, this,
                [](QNetworkReply::NetworkError error) {
            if (error != QNetworkReply::NoError)
                qCWarning(lcOdrsBackend, "Failed to fetch reviews: %s",
                          qPrintable(reply->errorString()));
        });
#endif
    connect(reply, &QNetworkReply::finished, this, [this, reply] {
        auto *review = qobject_cast<Review *>(reply->request().originatingObject());

        if (reply->error() != QNetworkReply::NoError) {
            qCWarning(lcOdrsBackend, "Failed to post review for \"%s\": %s",
                      qPrintable(review->resource()->appId()),
                      qPrintable(reply->errorString()));
            return;
        }

        QJsonParseError parseError;
        const auto json = QJsonDocument::fromJson(reply->readAll(), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            qCWarning(lcOdrsBackend, "Failed to post review for \"%s\": %s",
                      qPrintable(review->resource()->appId()),
                      qPrintable(parseError.errorString()));
            return;
        }

        Q_EMIT reviewSubmitted(review);

        ReviewPrivate::get(review)->setAlreadyVoted(true);

        const QJsonDocument document =
                review->getMetadataValue(QStringLiteral("ODRS::json")).toJsonDocument();
        parseReviews(document, review->resource());
    });
}

void OdrsBackend::parseReviews(const QJsonDocument &json, SoftwareResource *resource)
{
    if (resource->appId().isEmpty()) {
        qCWarning(lcOdrsBackend, "Cannot parse reviews for an invalid resource");
        return;
    }

    QJsonArray data = json.array();
    if (!data.isEmpty()) {
        QList<Review *> reviews;

        for (auto it = data.begin(); it != data.end(); it++) {
            const QJsonObject object = it->toObject();
            if (!object.isEmpty()) {
                int reviewId = object.value(QLatin1String("review_id")).toInt();

                bool alreadyExisting = false;
                Review *review = nullptr;

                if (reviewId > 0) {
                    for (auto *curReview : qAsConst(m_reviews)) {
                        if (curReview->id() == reviewId) {
                            alreadyExisting = true;
                            review = curReview;
                            break;
                        }
                    }
                }

                if (!review)
                    review = new Review();
                auto *dReview = ReviewPrivate::get(review);

                dReview->setResource(resource);
                dReview->setId(reviewId);

                QDateTime dateTime;
                dateTime.setSecsSinceEpoch(object.value(QLatin1String("date_created")).toInt());
                dReview->setCreationDate(dateTime);

                dReview->setRating(object.value(QLatin1String("rating")).toInt());

                if (object.contains(QLatin1String("score"))) {
                    dReview->setPriority(object.value(QLatin1String("score")).toInt());
                } else if (object.contains(QLatin1String("karma_up")) &&
                           object.contains(QLatin1String("karma_down"))) {
                    int karmaUp = object.value(QLatin1String("karma_up")).toInt();
                    int karmaDown = object.value(QLatin1String("karma_down")).toInt();

                    // Calculate average rating (from http://www.evanmiller.org/how-not-to-sort-by-average-rating.html)
                    qreal ku = qreal(karmaUp);
                    qreal kd = qreal(karmaDown);
                    qreal wilson = 0;
                    if (ku > 0 || kd > 0) {
                        wilson = ((ku + 1.9208f) / (ku + kd) - 1.96f * qSqrt((ku * kd) / (ku + kd) + 0.9604f) / (ku + kd)) / (1 + 3.8416f / (ku + kd));
                        wilson *= 100.0f;
                    }

                    dReview->setPriority(qFloor(wilson));
                }

                dReview->setReviewerId(object.value(QLatin1String("user_hash")).toString());
                dReview->setReviewerName(object.value(QLatin1String("user_display")).toString());
                dReview->setSummary(object.value(QLatin1String("summary")).toString());
                dReview->setDescription(object.value(QLatin1String("description")).toString());
                dReview->setVersion(object.value(QLatin1String("version")).toString());
                dReview->setAlreadyVoted(object.contains(QLatin1String("vote_id")));

                review->addMetadata(QStringLiteral("ODRS::user_skey"),
                                    object.value(QLatin1String("user_skey")).toVariant());
                review->addMetadata(QStringLiteral("ODRS::app_id"),
                                    object.value(QLatin1String("app_id")).toVariant());
                review->addMetadata(QStringLiteral("ODRS::review_id"), reviewId);

                resource->addMetadata(QStringLiteral("ODRS::user_skey"),
                                      object.value(QLatin1String("user_skey")).toVariant());

                if (!alreadyExisting) {
                    m_reviews.append(review);
                    SoftwareResourcePrivate::get(resource)->reviews.append(review);
                    Q_EMIT reviewAdded(review);
                }
            }
        }

        Q_EMIT resource->reviewsFetched();
    }
}

} // namespace AppCenter

} // namespace Liri
