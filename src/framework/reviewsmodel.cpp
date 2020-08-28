// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QtMath>

#include "reviewsmodel_p.h"

namespace Liri {

namespace AppCenter {

/*
 * ReviewsModelPrivate
 */

ReviewsModelPrivate::ReviewsModelPrivate(ReviewsModel *self)
    : q_ptr(self)
{
}

void ReviewsModelPrivate::handleReviewsFetched()
{
    Q_Q(ReviewsModel);

    // Adding all reviews at once is faster than adding one by one
    // when catching the SoftwareManager::reviewAdded(Review*) signal
    q->beginInsertRows(QModelIndex(), reviews.size(), reviews.size() + resource->reviews().size() - 1);
    reviews += resource->reviews();
    q->endInsertRows();
}

/*
 * ReviewsModel
 */

ReviewsModel::ReviewsModel(QObject *parent)
    : QAbstractListModel(parent)
    , d_ptr(new ReviewsModelPrivate(this))
{
}

ReviewsModel::~ReviewsModel()
{
    delete d_ptr;
}

QHash<int, QByteArray> ReviewsModel::roleNames() const
{
    QHash<int, QByteArray> roles = QAbstractItemModel::roleNames();
    roles.insert(CreationDateRole, "creationDate");
    roles.insert(RatingRole, "rating");
    roles.insert(PriorityRole, "priority");
    roles.insert(ReviewerIdRole, "reviewerId");
    roles.insert(ReviewerNameRole, "reviewerName");
    roles.insert(SummaryRole, "summary");
    roles.insert(DescriptionRole, "description");
    roles.insert(VersionRole, "version");
    roles.insert(HasAlreadyVotedRole, "hasAlreadyVoted");
    roles.insert(MetadataRole, "metadata");
    return roles;
}

int ReviewsModel::rowCount(const QModelIndex &parent) const
{
    Q_D(const ReviewsModel);

    // We return the count only for the root node (which has an invalid parent),
    // in other cases we return 0 so that it does not become a tree model
    if (parent.isValid())
        return 0;

    return d->reviews.size();
}

QVariant ReviewsModel::data(const QModelIndex &index, int role) const
{
    Q_D(const ReviewsModel);

    if (!index.isValid())
        return QVariant();
    if (d->reviews.size() == 0)
        return QVariant();

    Review *review = d->reviews.at(index.row());
    if (!review)
        return QVariant();

    switch (role) {
    case CreationDateRole:
        return review->creationDate();
    case RatingRole:
        return review->rating();
    case PriorityRole:
        return review->priority();
    case ReviewerIdRole:
        return review->reviewerId();
    case ReviewerNameRole:
        return review->reviewerName();
    case SummaryRole:
        return review->summary();
    case DescriptionRole:
        return review->description();
    case VersionRole:
        return review->version();
    case HasAlreadyVotedRole:
        return review->hasAlreadyVoted();
    case MetadataRole:
        return review->metadata();
    }

    return QVariant();
}

SoftwareResource *ReviewsModel::resource() const
{
    Q_D(const ReviewsModel);
    return d->resource;
}

void ReviewsModel::setResource(SoftwareResource *resource)
{
    Q_D(ReviewsModel);

    if (d->resource == resource)
        return;

    if (d->resource)
        disconnect(resource, SIGNAL(reviewsFetched()),
                   this, SLOT(handleReviewsFetched()));

    beginRemoveRows(QModelIndex(), 0, d->reviews.size() - 1);
    d->resource = resource;
    d->reviews.clear();
    connect(resource, SIGNAL(reviewsFetched()),
            this, SLOT(handleReviewsFetched()));
    resource->fetchReviews();
    endRemoveRows();

    Q_EMIT resourceChanged();
}

} // namespace AppCenter

} // namespace Liri

#include "moc_reviewsmodel.cpp"
