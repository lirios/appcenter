// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "review_p.h"

namespace Liri {

namespace AppCenter {

/*
 * ReviewPrivate
 */

ReviewPrivate::ReviewPrivate(Review *self)
    : q_ptr(self)
{
}

void ReviewPrivate::setResource(SoftwareResource *value)
{
    Q_Q(Review);

    if (resource == value)
        return;

    resource = value;
    Q_EMIT q->resourceChanged();
}

void ReviewPrivate::setId(int value)
{
    Q_Q(Review);

    if (id == value)
        return;

    id = value;
    Q_EMIT q->idChanged();
}

void ReviewPrivate::setCreationDate(const QDateTime &value)
{
    Q_Q(Review);

    if (dateCreated == value)
        return;

    dateCreated = value;
    Q_EMIT q->creationDateChanged();
}

void ReviewPrivate::setRating(int value)
{
    Q_Q(Review);

    if (rating == value)
        return;

    rating = value;
    Q_EMIT q->ratingChanged();
}

void ReviewPrivate::setPriority(int value)
{
    Q_Q(Review);

    if (priority == value)
        return;

    priority = value;
    Q_EMIT q->priorityChanged();
}

void ReviewPrivate::setKarmaUp(int value)
{
    Q_Q(Review);

    if (karmaUp == value)
        return;

    karmaUp = value;
    Q_EMIT q->karmaUpChanged();
}

void ReviewPrivate::setKarmaDown(int value)
{
    Q_Q(Review);

    if (karmaDown == value)
        return;

    karmaDown = value;
    Q_EMIT q->karmaDownChanged();
}

void ReviewPrivate::setReviewerId(const QString &value)
{
    Q_Q(Review);

    if (reviewerId == value)
        return;

    reviewerId = value;
    Q_EMIT q->reviewerIdChanged();
}

void ReviewPrivate::setReviewerName(const QString &value)
{
    Q_Q(Review);

    if (reviewerName == value)
        return;

    reviewerName = value;
    Q_EMIT q->reviewerNameChanged();
}

void ReviewPrivate::setSummary(const QString &value)
{
    Q_Q(Review);

    if (summary == value)
        return;

    summary = value;
    Q_EMIT q->summaryChanged();
}

void ReviewPrivate::setDescription(const QString &value)
{
    Q_Q(Review);

    if (description == value)
        return;

    description = value;
    Q_EMIT q->descriptionChanged();
}

void ReviewPrivate::setVersion(const QString &value)
{
    Q_Q(Review);

    if (version == value)
        return;

    version = value;
    Q_EMIT q->versionChanged();
}

void ReviewPrivate::setSelfMade(bool value)
{
    Q_Q(Review);

    if (selfMade == value)
        return;

    selfMade = value;
    Q_EMIT q->selfMadeChanged();
}

void ReviewPrivate::setAlreadyVoted(bool value)
{
    Q_Q(Review);

    if (alreadyVoted == value)
        return;

    alreadyVoted = value;
    Q_EMIT q->hasAlreadyVotedChanged();
}

/*
 * Review
 */

Review::Review(QObject *parent)
    : QObject(parent)
    , d_ptr(new ReviewPrivate(this))
{
}

Review::~Review()
{
    delete d_ptr;
}

ReviewsBackend *Review::backend() const
{
    Q_D(const Review);
    return d->backend;
}

SoftwareResource *Review::resource() const
{
    Q_D(const Review);
    return d->resource;
}

int Review::id() const
{
    Q_D(const Review);
    return d->id;
}

QDateTime Review::creationDate() const
{
    Q_D(const Review);
    return d->dateCreated;
}

int Review::rating() const
{
    Q_D(const Review);
    return d->rating;
}

int Review::priority() const
{
    Q_D(const Review);
    return d->priority;
}

QString Review::reviewerId() const
{
    Q_D(const Review);
    return d->reviewerId;
}

QString Review::reviewerName() const
{
    Q_D(const Review);
    return d->reviewerName;
}

QString Review::summary() const
{
    Q_D(const Review);
    return d->summary;
}

QString Review::description() const
{
    Q_D(const Review);
    return d->description;
}

QString Review::version() const
{
    Q_D(const Review);
    return d->version;
}

bool Review::isSelfMade() const
{
    Q_D(const Review);
    return d->selfMade;
}

bool Review::hasAlreadyVoted() const
{
    Q_D(const Review);
    return d->alreadyVoted;
}

int Review::karmaUp() const
{
    Q_D(const Review);
    return d->karmaUp;
}

int Review::karmaDown() const
{
    Q_D(const Review);
    return d->karmaDown;
}

QVariantMap Review::metadata() const
{
    Q_D(const Review);
    return d->metadata;
}

void Review::addMetadata(const QString &key, const QVariant &value)
{
    Q_D(Review);

    d->metadata.insert(key, value);
    Q_EMIT metadataChanged();
}

QVariant Review::getMetadataValue(const QString &key)
{
    Q_D(const Review);
    return d->metadata.value(key);
}

void Review::upVote()
{
    Q_D(Review);
    d->backend->upVoteReview(this);
}

void Review::downVote()
{
    Q_D(Review);
    d->backend->downVoteReview(this);
}

void Review::report()
{
    Q_D(Review);
    d->backend->reportReview(this);
}

void Review::dismiss()
{
    Q_D(Review);
    d->backend->dismissReview(this);
}

void Review::remove()
{
    Q_D(Review);
    d->backend->removeReview(this);
}

} // namespace AppCenter

} // namespace Liri
