// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "transactionsmanager_p.h"

namespace Liri {

namespace AppCenter {

Q_GLOBAL_STATIC(TransactionsManager, s_transactionsManager)

TransactionsManagerPrivate::TransactionsManagerPrivate()
{
}

TransactionsManagerPrivate::~TransactionsManagerPrivate()
{
    qDeleteAll(transactions);
}

TransactionsManager::TransactionsManager(QObject *parent)
    : QObject(parent)
    , d_ptr(new TransactionsManagerPrivate())
{
}

TransactionsManager::~TransactionsManager()
{
    delete d_ptr;
}

TransactionsManager *TransactionsManager::instance()
{
    return s_transactionsManager();
}

void TransactionsManager::addTransaction(Transaction *transaction)
{
    Q_D(TransactionsManager);

    if (d->transactions.contains(transaction))
        return;

    d->transactions.append(transaction);
    Q_EMIT transactionAdded(transaction);
}

void TransactionsManager::removeTransaction(Transaction *transaction)
{
    Q_D(TransactionsManager);

    if (!d->transactions.contains(transaction))
        return;

    if (d->transactions.removeOne(transaction))
        Q_EMIT transactionRemoved(transaction);
}

} // namespace AppCenter

} // namespace Liri
