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
