// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#ifndef LIRI_FLATPAKAPPSTREAMJOB_H
#define LIRI_FLATPAKAPPSTREAMJOB_H

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

#endif // LIRI_FLATPAKAPPSTREAMJOB_H
