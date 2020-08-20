// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include <QVariantMap>

#include "rating_p.h"

namespace Liri {

namespace AppCenter {

Rating::Rating(const QString &appId, quint64 numVotes,
               qreal rating, QObject *parent)
    : QObject(parent)
    , d_ptr(new RatingPrivate())
{
    d_ptr->appId = appId;
    d_ptr->numVotes = numVotes;
    d_ptr->rating = rating;
}

Rating::Rating(const QString &appId, quint64 numVotes,
               const QVariantMap &data, QObject *parent)
    : QObject(parent)
    , d_ptr(new RatingPrivate())
{
    d_ptr->appId = appId;
    d_ptr->numVotes = numVotes;

    d_ptr->star0 = qMax<quint64>(0, data.value(QStringLiteral("star0")).toInt());
    d_ptr->star1 = qMax<quint64>(0, data.value(QStringLiteral("star1")).toInt());
    d_ptr->star2 = qMax<quint64>(0, data.value(QStringLiteral("star2")).toInt());
    d_ptr->star3 = qMax<quint64>(0, data.value(QStringLiteral("star3")).toInt());
    d_ptr->star4 = qMax<quint64>(0, data.value(QStringLiteral("star4")).toInt());
    d_ptr->star5 = qMax<quint64>(0, data.value(QStringLiteral("star5")).toInt());

    int stars = d_ptr->star1 + (d_ptr->star2 * 2) + (d_ptr->star3 * 3) +
            (d_ptr->star4 * 4) + (d_ptr->star5 * 5);
    d_ptr->rating = stars / qMax<qreal>(1.0, qreal(numVotes));
}

Rating::~Rating()
{
    delete d_ptr;
}

QString Rating::appId() const
{
    Q_D(const Rating);
    return d->appId;
}

quint64 Rating::numVotes() const
{
    Q_D(const Rating);
    return d->numVotes;
}

qreal Rating::rating() const
{
    Q_D(const Rating);
    return d->rating;
}

quint64 Rating::star0() const
{
    Q_D(const Rating);
    return d->star0;
}

quint64 Rating::star1() const
{
    Q_D(const Rating);
    return d->star1;
}

quint64 Rating::star2() const
{
    Q_D(const Rating);
    return d->star2;
}

quint64 Rating::star3() const
{
    Q_D(const Rating);
    return d->star3;
}

quint64 Rating::star4() const
{
    Q_D(const Rating);
    return d->star4;
}

quint64 Rating::star5() const
{
    Q_D(const Rating);
    return d->star5;
}

} // namespace AppCenter

} // namespace Liri
