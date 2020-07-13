// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEWSMODEL_H
#define LIRI_APPCENTER_REVIEWSMODEL_H

#include <QAbstractListModel>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class ReviewsModelPrivate;

class LIRIAPPCENTER_EXPORT ReviewsModel : public QAbstractListModel
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(ReviewsModel)
    Q_DISABLE_COPY(ReviewsModel)
    Q_PROPERTY(SoftwareResource *resource READ resource WRITE setResource NOTIFY resourceChanged)
    Q_PROPERTY(int count READ rowCount NOTIFY countChanged)
public:
    enum Roles {
        ReviewRole = Qt::UserRole + 1,
        CreationDateRole,
        RatingRole,
        PriorityRole,
        KarmaUpRole,
        KarmaDownRole,
        ReviewerIdRole,
        ReviewerNameRole,
        SummaryRole,
        DescriptionRole,
        VersionRole,
        SelfMadeRole,
        HasAlreadyVotedRole,
        MetadataRole,
    };

    explicit ReviewsModel(QObject *parent = nullptr);
    ~ReviewsModel();

    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    SoftwareResource *resource() const;
    void setResource(SoftwareResource *resource);

Q_SIGNALS:
    void resourceChanged();
    void countChanged();
    void reviewsFetched();

private:
    ReviewsModelPrivate *const d_ptr;

    Q_PRIVATE_SLOT(d_func(), void handleReviewsFetched())
    Q_PRIVATE_SLOT(d_func(), void handleReviewSubmitted(Review*))
    Q_PRIVATE_SLOT(d_func(), void handleReviewRemoved(Review*))
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_REVIEWSMODEL_H
