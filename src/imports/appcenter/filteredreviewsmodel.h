// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef FILTEREDREVIEWSMODEL_H
#define FILTEREDREVIEWSMODEL_H

#include <QSortFilterProxyModel>

#include <LiriAppCenter/ReviewsModel>
#include <LiriAppCenter/SoftwareResource>

using namespace Liri::AppCenter;

class FilteredReviewsModel : public QSortFilterProxyModel
{
    Q_OBJECT
    Q_PROPERTY(SoftwareResource *resource READ resource WRITE setResource NOTIFY resourceChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    explicit FilteredReviewsModel(QObject *parent = nullptr);

    SoftwareResource *resource() const;
    void setResource(SoftwareResource *resource);

Q_SIGNALS:
    void resourceChanged();
    void countChanged();
    void reviewsFetched();

protected:
    bool lessThan(const QModelIndex &source_left, const QModelIndex &source_right) const override;

private:
    ReviewsModel *m_reviewsModel;
};

#endif // FILTEREDREVIEWSMODEL_H
