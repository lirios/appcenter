// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
