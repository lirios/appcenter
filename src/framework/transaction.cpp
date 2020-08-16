/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:LGPLv3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include "softwareresource.h"
#include "transaction.h"
#include "transaction_p.h"
#include "transactionsmanager.h"

namespace Liri {

namespace AppCenter {

TransactionPrivate::TransactionPrivate()
{
}

Transaction::Transaction(Type type, const QString &name,
                         const QString &description,
                         SoftwareResource *resource,
                         bool cancellable,
                         QObject *parent)
    : QObject(parent)
    , d_ptr(new TransactionPrivate())
{
    Q_D(Transaction);
    d->type = type;
    d->name = name;
    d->description = description;
    d->resource = resource;
    d->cancellable = cancellable;

    TransactionsManager::instance()->addTransaction(this);
}

Transaction::~Transaction()
{
    TransactionsManager::instance()->removeTransaction(this);
    delete d_ptr;
}

SoftwareResource *Transaction::resource() const
{
    Q_D(const Transaction);
    return d->resource;
}

Transaction::Type Transaction::type() const
{
    Q_D(const Transaction);
    return d->type;
}

QString Transaction::name() const
{
    Q_D(const Transaction);
    return d->name;
}

QString Transaction::description() const
{
    Q_D(const Transaction);
    return d->description;
}

Transaction::Status Transaction::status() const
{
    Q_D(const Transaction);
    return d->status;
}

void Transaction::setStatus(Transaction::Status status)
{
    Q_D(Transaction);

    if (d->status == status)
        return;

    d->status = status;
    Q_EMIT statusChanged();

    if (d->status == Succeeded)
        Q_EMIT succeeded();
    else if (d->status == Failed)
        Q_EMIT failed();

    if (d->status == Succeeded || d->status == Failed || d->status == Cancelled) {
        d->cancellable = false;
        Q_EMIT cancellableChanged();
        TransactionsManager::instance()->removeTransaction(this);
    }
}

QString Transaction::errorMessage() const
{
    Q_D(const Transaction);
    return d->errorMessage;
}

void Transaction::setErrorMessage(const QString &message)
{
    Q_D(Transaction);

    if (d->errorMessage == message)
        return;

    d->errorMessage = message;
    Q_EMIT errorMessageChanged();
}

bool Transaction::isCancellable() const
{
    Q_D(const Transaction);
    return d->cancellable;
}

int Transaction::progress() const
{
    Q_D(const Transaction);
    return d->progress;
}

void Transaction::setProgress(int value)
{
    Q_D(Transaction);

    if (d->progress == value)
        return;

    d->progress = qBound(0, value, 100);
    Q_EMIT progressChanged();
}

bool Transaction::isVisible() const
{
    Q_D(const Transaction);
    return d->visible;
}

} // namespace AppCenter

} // namespace Liri
