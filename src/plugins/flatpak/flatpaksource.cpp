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

#include "flatpakbackend.h"
#include "flatpaksource.h"

FlatpakSource::FlatpakSource(FlatpakInstallation *installation, FlatpakRemote *remote, QObject *parent)
    : Liri::AppCenter::SoftwareSource(parent)
    , m_installation(installation)
    , m_remote(remote)
{
    connect(this, &FlatpakSource::enabledChanged, this, [&] {
        if (!m_remote)
            return;

        bool enabled = flatpak_remote_get_disabled(m_remote);
        if (enabled != isEnabled()) {
            flatpak_remote_set_disabled(m_remote, !isEnabled());
            flatpak_installation_modify_remote(m_installation, m_remote, nullptr, nullptr);
        }
    });
}

FlatpakSource::~FlatpakSource()
{
    g_object_unref(m_remote);
    m_remote = nullptr;
}

FlatpakInstallation *FlatpakSource::installation() const
{
    return m_installation;
}

void FlatpakSource::setInstallation(FlatpakInstallation *installation)
{
    m_installation = installation;
}

FlatpakRemote *FlatpakSource::remote() const
{
    return m_remote;
}

void FlatpakSource::setRemote(FlatpakRemote *remote)
{
    m_remote = remote;
}

QDir FlatpakSource::appStreamDir() const
{
    g_autoptr(GFile) dir = flatpak_remote_get_appstream_dir(m_remote, nullptr);
    if (!dir) {
        qCWarning(lcFlatpakBackend) << "No AppStream directory for" << name();
        return QDir();
    }

    return QDir(QString::fromUtf8(g_file_get_path(dir)));
}
