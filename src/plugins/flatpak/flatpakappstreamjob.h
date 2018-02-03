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

#ifndef FLATPAKAPPSTREAMJOB_H
#define FLATPAKAPPSTREAMJOB_H

#include <QThread>

#include "flatpakpluginbase.h"

class FlatpakAppStreamJob : public QThread
{
    Q_OBJECT
public:
    explicit FlatpakAppStreamJob(FlatpakInstallation *installation,
                                 FlatpakRemote *remote,
                                 QObject *parent = nullptr);
    ~FlatpakAppStreamJob();

    void run() override;
    void cancel();

Q_SIGNALS:
    void succeeded(FlatpakInstallation *installation, FlatpakRemote *remote);
    void failed(const QString &errorMessage);

private:
    FlatpakInstallation *m_installation = nullptr;
    FlatpakRemote *m_remote = nullptr;
    GCancellable *m_cancellable = nullptr;
};

#endif // FLATPAKAPPSTREAMJOB_H
