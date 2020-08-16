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

#ifndef FLATPAKTRANSACTION_H
#define FLATPAKTRANSACTION_H

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

#endif // FLATPAKTRANSACTION_H
