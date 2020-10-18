// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: LGPL-3.0-or-later

#include "flatpakappstreamjob.h"
#include "flatpakplugin.h"

FlatpakAppStreamJob::FlatpakAppStreamJob(FlatpakInstallation *installation, FlatpakRemote *remote, QObject *parent)
    : QThread(parent)
    , m_installation(installation)
    , m_remote(remote)
    , m_cancellable(g_cancellable_new())
{
}

FlatpakAppStreamJob::~FlatpakAppStreamJob()
{
    g_object_unref(m_cancellable);
}

void FlatpakAppStreamJob::run()
{
    g_autoptr(GError) error = nullptr;

    const char *name = flatpak_remote_get_name(m_remote);

    if (!flatpak_installation_update_remote_sync(m_installation, name, m_cancellable, &error)) {
        Q_EMIT failed(QString::fromUtf8(error->message));
        return;
    }

    if (!flatpak_installation_update_appstream_full_sync(m_installation, name, nullptr, nullptr, nullptr, nullptr, m_cancellable, &error))
        Q_EMIT failed(QString::fromUtf8(error->message));
    else
        Q_EMIT succeeded(m_installation, m_remote);
}

void FlatpakAppStreamJob::cancel()
{
    g_cancellable_cancel(m_cancellable);
}
