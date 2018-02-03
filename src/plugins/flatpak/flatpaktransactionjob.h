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

#ifndef FLATPAKTRANSACTIONJOB_H
#define FLATPAKTRANSACTIONJOB_H

#include <QThread>

#include <LiriAppCenter/Transaction>

#include "flatpakresource.h"

class FlatpakTransactionJob : public QThread
{
    Q_OBJECT
    Q_PROPERTY(int progress READ progress WRITE setProgress NOTIFY progressChanged)
public:
    explicit FlatpakTransactionJob(FlatpakResource *app,
                                   Liri::AppCenter::Transaction::Type type,
                                   QObject *parent = nullptr);
    ~FlatpakTransactionJob();

    int progress() const;
    void setProgress(int progress);

    void run() override;
    void cancel();

Q_SIGNALS:
    void progressChanged(int progress);
    void succeeded();
    void failed(const QString &message);
    void cancelled();

private:
    FlatpakResource *m_app = nullptr;
    Liri::AppCenter::Transaction::Type m_type;
    GCancellable *m_cancellable = nullptr;
    int m_progress = 0;
};

#endif // FLATPAKTRANSACTIONJOB_H
