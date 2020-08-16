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
        setStatus(Transaction::Failed);
        setErrorMessage(message);
    });
    m_job->start();
}

void LiriFlatpakTransaction::cancel()
{
    if (isCancellable())
        m_job->cancel();
}
