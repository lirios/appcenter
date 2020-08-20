// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_REVIEW_H
#define LIRI_APPCENTER_REVIEW_H

#include <QDateTime>
#include <QObject>

#include <LiriAppCenter/SoftwareResource>

namespace Liri {

namespace AppCenter {

class ReviewPrivate;

class LIRIAPPCENTER_EXPORT Review : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Review)
    Q_PROPERTY(SoftwareResource *resource READ resource NOTIFY resourceChanged)
    Q_PROPERTY(int id READ id NOTIFY idChanged)
    Q_PROPERTY(QDateTime creationDate READ creationDate NOTIFY creationDateChanged)
    Q_PROPERTY(int rating READ rating NOTIFY ratingChanged)
    Q_PROPERTY(int priority READ priority NOTIFY priorityChanged)
    Q_PROPERTY(QString reviewerId READ reviewerId NOTIFY reviewerIdChanged)
    Q_PROPERTY(QString reviewerName READ reviewerName NOTIFY reviewerNameChanged)
    Q_PROPERTY(QString summary READ summary NOTIFY summaryChanged)
    Q_PROPERTY(QString description READ description NOTIFY descriptionChanged)
    Q_PROPERTY(QString version READ version NOTIFY versionChanged)
    Q_PROPERTY(bool hasAlreadyVoted READ hasAlreadyVoted NOTIFY hasAlreadyVotedChanged)
    Q_PROPERTY(QVariantMap metadata READ metadata NOTIFY metadataChanged)
public:
    explicit Review(QObject *parent = nullptr);
    ~Review();

    SoftwareResource *resource() const;

    int id() const;

    QDateTime creationDate() const;

    int rating() const;
    int priority() const;

    QString reviewerId() const;
    QString reviewerName() const;

    QString summary() const;
    QString description() const;

    QString version() const;

    bool hasAlreadyVoted() const;

    QVariantMap metadata() const;
    void addMetadata(const QString &key, const QVariant &value);
    Q_INVOKABLE QVariant getMetadataValue(const QString &key);

Q_SIGNALS:
    void resourceChanged();
    void idChanged();
    void creationDateChanged();
    void ratingChanged();
    void priorityChanged();
    void reviewerIdChanged();
    void reviewerNameChanged();
    void summaryChanged();
    void descriptionChanged();
    void versionChanged();
    void hasAlreadyVotedChanged();
    void metadataChanged();

private:
    ReviewPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_REVIEW_H
