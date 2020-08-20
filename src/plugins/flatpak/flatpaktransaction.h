// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKTRANSACTION_H
#define LIRI_FLATPAKTRANSACTION_H

#include <LiriAppCenter/Transaction>

#include "flatpakresource.h"
#include "flatpaktransactionjob.h"

class LiriFlatpakTransaction : public Liri::AppCenter::Transaction
{
    Q_OBJECT
public:
    explicit LiriFlatpakTransaction(Liri::AppCenter::Transaction::Type type,
                                    const QString &name,
                                    const QString &description,
                                    Liri::AppCenter::SoftwareResource *resource,
                                    bool cancellable = true,
                                    QObject *parent = nullptr);

    Q_INVOKABLE void run() override;
    Q_INVOKABLE void cancel() override;

private:
    FlatpakResource *m_resource = nullptr;
    FlatpakTransactionJob *m_job = nullptr;
};

#endif // LIRI_FLATPAKTRANSACTION_H
