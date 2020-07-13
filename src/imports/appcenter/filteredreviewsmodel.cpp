// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QDateTime>

#include "filteredreviewsmodel.h"

FilteredReviewsModel::FilteredReviewsModel(QObject *parent)
    : QSortFilterProxyModel(parent)
    , m_reviewsModel(new ReviewsModel(this))
{
    connect(this, &QAbstractListModel::rowsInserted,
            this, &FilteredReviewsModel::countChanged);
    connect(this, &QAbstractListModel::rowsRemoved,
            this, &FilteredReviewsModel::countChanged);
    connect(m_reviewsModel, &ReviewsModel::resourceChanged,
            this, &FilteredReviewsModel::resourceChanged);
    connect(m_reviewsModel, &ReviewsModel::reviewsFetched,
            this, &FilteredReviewsModel::reviewsFetched);

    setSourceModel(m_reviewsModel);
    setDynamicSortFilter(true);
    setSortRole(ReviewsModel::CreationDateRole);
    setSortLocaleAware(true);
    sort(0);
}

SoftwareResource *FilteredReviewsModel::resource() const
{
    return m_reviewsModel->resource();
}

void FilteredReviewsModel::setResource(SoftwareResource *resource)
{
    m_reviewsModel->setResource(resource);
}

bool FilteredReviewsModel::lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const
{
    // Sort from the most recent to the least recent (descending order)
    auto leftDateTime = sourceModel()->data(source_left, sortRole()).toDateTime();
    auto rightDateTime = sourceModel()->data(source_right, sortRole()).toDateTime();
    return leftDateTime > rightDateTime;
}

