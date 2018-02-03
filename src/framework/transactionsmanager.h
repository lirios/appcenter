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

#ifndef LIRIAPPCENTERTRANSACTIONSMANAGER_H
#define LIRIAPPCENTERTRANSACTIONSMANAGER_H

#include <LiriAppCenter/Transaction>

namespace Liri {

namespace AppCenter {

class TransactionsManagerPrivate;

class LIRIAPPCENTER_EXPORT TransactionsManager : public QObject
{
    Q_OBJECT
    Q_DECLARE_PRIVATE(TransactionsManager)
    Q_DISABLE_COPY(TransactionsManager)
public:
    explicit TransactionsManager(QObject *parent = nullptr);
    ~TransactionsManager();

    static TransactionsManager *instance();

    void addTransaction(Transaction *transaction);
    void removeTransaction(Transaction *transaction);

Q_SIGNALS:
    void transactionAdded(Transaction *transaction);
    void transactionRemoved(Transaction *transaction);

private:
    TransactionsManagerPrivate *const d_ptr;
};

} // namespace AppCenter

} // namespace Liri

#endif // LIRIAPPCENTERTRANSACTIONSMANAGER_H
