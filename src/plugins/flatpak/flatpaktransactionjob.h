// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKTRANSACTIONJOB_H
#define LIRI_FLATPAKTRANSACTIONJOB_H

#include <QThread>

#include <LiriAppCenter/Transaction>

#include "flatpakresource.h"

class FlatpakTransactionJob : public QThread
{
    Q_OBJECT
    Q_PROPERTY(Liri::AppCenter::SoftwareResource *resource READ resource CONSTANT)
    Q_PROPERTY(Liri::AppCenter::Transaction *transaction READ transaction CONSTANT)
    Q_PROPERTY(bool isCancellable READ isCancellable CONSTANT)
public:
    explicit FlatpakTransactionJob(FlatpakResource *app,
                                   Liri::AppCenter::Transaction *transaction,
                                   bool cancellable = true,
                                   QObject *parent = nullptr);
    ~FlatpakTransactionJob();

    Liri::AppCenter::SoftwareResource *resource() const;
    Liri::AppCenter::Transaction *transaction() const;
    bool isCancellable() const;

    void run() override;
    void cancel();

Q_SIGNALS:
    void succeeded();
    void failed(const QString &message);
    void cancelled();

private:
    FlatpakResource *m_app = nullptr;
    Liri::AppCenter::Transaction *m_transaction = nullptr;
    GCancellable *m_cancellable = nullptr;
    int m_progress = 0;
};

#endif // LIRI_FLATPAKTRANSACTIONJOB_H
