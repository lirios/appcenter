// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEW_P_H
#define LIRI_APPCENTER_REVIEW_P_H

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

#include <QVariantMap>

#include <LiriAppCenter/ReviewsBackend>
#include <LiriAppCenter/Review>

namespace Liri {

namespace AppCenter {

class LIRIAPPCENTER_EXPORT ReviewPrivate
{
    Q_DECLARE_PUBLIC(Review)
public:
    explicit ReviewPrivate(Review *self);

    void setResource(SoftwareResource *value);
    void setId(int value);
    void setCreationDate(const QDateTime &value);
    void setRating(int value);
    void setPriority(int value);
    void setKarmaUp(int value);
    void setKarmaDown(int value);
    void setReviewerId(const QString &value);
    void setReviewerName(const QString &value);
    void setSummary(const QString &value);
    void setDescription(const QString &value);
    void setVersion(const QString &value);
    void setSelfMade(bool value);
    void setAlreadyVoted(bool value);

    static ReviewPrivate *get(Review *review) { return review->d_func(); }

    ReviewsBackend *backend = nullptr;
    SoftwareResource *resource = nullptr;
    int id = 0;
    QDateTime dateCreated;
    int rating = 0;
    int priority = 0;
    int karmaUp = 0;
    int karmaDown = 0;
    QString reviewerId;
    QString reviewerName;
    QString summary;
    QString description;
    QString version;
    bool selfMade = false;
    bool alreadyVoted = false;
    QVariantMap metadata;

protected:
    Review *q_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_REVIEW_P_H
