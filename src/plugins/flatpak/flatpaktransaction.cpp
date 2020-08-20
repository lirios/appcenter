// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "flatpaktransaction.h"

using namespace Liri::AppCenter;

LiriFlatpakTransaction::LiriFlatpakTransaction(Type type, const QString &name,
                                               const QString &description,
                                               SoftwareResource *resource,
                                               bool cancellable,
                                               QObject *parent)
    : Transaction(type, name, description, resource, cancellable, parent)
    , m_resource(qobject_cast<FlatpakResource *>(resource))
    , m_job(new FlatpakTransactionJob(m_resource, this, cancellable))
{
}

void LiriFlatpakTransaction::run()
{
    connect(m_job, &FlatpakTransactionJob::cancelled, this, [&] {
        setStatus(Transaction::Cancelled);
    });
    connect(m_job, &FlatpakTransactionJob::succeeded, this, [&] {
        setStatus(Transaction::Succeeded);
    });
    connect(m_job, &FlatpakTransactionJob::failed, this, [&](const QString &message) {
        setErrorMessage(message);
        setStatus(Transaction::Failed);
    });
    m_job->start();
}

void LiriFlatpakTransaction::cancel()
{
    if (isCancellable())
        m_job->cancel();
}
