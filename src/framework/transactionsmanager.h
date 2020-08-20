// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

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
