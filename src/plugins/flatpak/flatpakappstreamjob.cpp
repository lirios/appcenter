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

    if (!flatpak_installation_update_appstream_sync(m_installation, name, nullptr, nullptr, m_cancellable, &error)) {
        qCWarning(lcFlatpakBackend, "Failed to update AppStream metadata for \"%s\": %s",
                  name, error->message);
        Q_EMIT failed(QString::fromUtf8(error->message));
    } else {
        Q_EMIT succeeded(m_installation, m_remote);
    }
}

void FlatpakAppStreamJob::cancel()
{
    g_cancellable_cancel(m_cancellable);
}
