/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#include "transactionsmanager.h"
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
