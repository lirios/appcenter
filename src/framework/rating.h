// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_APPCENTER_RATING_H
#define LIRI_APPCENTER_RATING_H

#include <QObject>

#include <LiriAppCenter/liriappcenterglobal.h>

namespace Liri {

namespace AppCenter {

class RatingPrivate;

class LIRIAPPCENTER_EXPORT Rating : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(Rating)
    Q_PROPERTY(QString appId READ appId CONSTANT)
    Q_PROPERTY(quint64 numVotes READ numVotes CONSTANT)
    Q_PROPERTY(qreal rating READ rating CONSTANT)
    Q_PROPERTY(quint64 star0 READ star0 CONSTANT)
    Q_PROPERTY(quint64 star1 READ star1 CONSTANT)
    Q_PROPERTY(quint64 star2 READ star2 CONSTANT)
    Q_PROPERTY(quint64 star3 READ star3 CONSTANT)
    Q_PROPERTY(quint64 star4 READ star4 CONSTANT)
    Q_PROPERTY(quint64 star5 READ star5 CONSTANT)
public:
    explicit Rating(const QString &appId,
                    quint64 numVotes, qreal rating,
                    QObject *parent = nullptr);
    explicit Rating(const QString &appId, quint64 numVotes,
                    const QVariantMap &data,
                    QObject *parent = nullptr);
    ~Rating();

    QString appId() const;
    quint64 numVotes() const;
    qreal rating() const;

    quint64 star0() const;
    quint64 star1() const;
    quint64 star2() const;
    quint64 star3() const;
    quint64 star4() const;
    quint64 star5() const;

private:
    RatingPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRI_APPCENTER_RATING_H
