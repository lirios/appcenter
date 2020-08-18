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

FlatpakSource::FlatpakSource(Liri::AppCenter::Backend *backend,
                             FlatpakInstallation *installation,
                             FlatpakRemote *remote,
                             QObject *parent)
    : Liri::AppCenter::SoftwareSource(backend, parent)
    , m_installation(installation)
    , m_remote(remote)
    , m_cancellable(g_cancellable_new())
{
}

FlatpakSource::~FlatpakSource()
{
    g_cancellable_cancel(m_cancellable);
    g_object_unref(m_cancellable);
    m_cancellable = nullptr;

    g_object_unref(m_remote);
    m_remote = nullptr;
}

QString FlatpakSource::name() const
{
    return QString::fromUtf8(flatpak_remote_get_name(m_remote));
}

QString FlatpakSource::title() const
{
    Q_ASSERT(m_remote);
    return QString::fromUtf8(flatpak_remote_get_title(m_remote));
}

void FlatpakSource::setTitle(const QString &value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_title(m_remote, value.toUtf8().constData());

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT titleChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set title on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

QString FlatpakSource::comment() const
{
    Q_ASSERT(m_remote);
    return QString::fromUtf8(flatpak_remote_get_comment(m_remote));
}

void FlatpakSource::setComment(const QString &value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_comment(m_remote, value.toUtf8().constData());

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT commentChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set comment on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

QString FlatpakSource::description() const
{
    Q_ASSERT(m_remote);
    return QString::fromUtf8(flatpak_remote_get_description(m_remote));
}

void FlatpakSource::setDescription(const QString &value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_description(m_remote, value.toUtf8().constData());

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT descriptionChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set description on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

QString FlatpakSource::section() const
{
    return QStringLiteral("Flatpak");
}

bool FlatpakSource::isEnabled() const
{
    Q_ASSERT(m_remote);
    return !flatpak_remote_get_disabled(m_remote);
}

void FlatpakSource::setEnabled(bool value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_disabled(m_remote, !value);

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT enabledChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to enable or disable source \"%s\": %s",
                  qPrintable(name()), error->message);
}

bool FlatpakSource::gpgVerify() const
{
    Q_ASSERT(m_remote);
    return flatpak_remote_get_gpg_verify(m_remote);
}

void FlatpakSource::setGpgVerify(bool value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_gpg_verify(m_remote, value);

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT gpgVerifyChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set GPG verification on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

QUrl FlatpakSource::url() const
{
    Q_ASSERT(m_remote);
    return QUrl(QString::fromUtf8(flatpak_remote_get_url(m_remote)));
}

void FlatpakSource::setUrl(const QUrl &value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_url(m_remote, value.toString().toUtf8().constData());

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT urlChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set URL on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

QUrl FlatpakSource::iconUrl() const
{
    Q_ASSERT(m_remote);
    return QUrl(QString::fromUtf8(flatpak_remote_get_icon(m_remote)));
}

void FlatpakSource::setIconUrl(const QUrl &value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_icon(m_remote, value.toString().toUtf8().constData());

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT iconUrlChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set icon URL on source \"%s\": %s",
                  qPrintable(name()), error->message);
}

int FlatpakSource::priority() const
{
    Q_ASSERT(m_remote);
    return flatpak_remote_get_prio(m_remote);
}

void FlatpakSource::setPriority(int value)
{
    Q_ASSERT(m_remote);
    Q_ASSERT(m_installation);

    flatpak_remote_set_prio(m_remote, value);

    g_autoptr(GError) error = nullptr;
    if (flatpak_installation_modify_remote(m_installation, m_remote, m_cancellable, &error))
        Q_EMIT priorityChanged();
    else
        qCWarning(lcFlatpakBackend, "Unable to set priority to source \"%s\": %s",
                  qPrintable(name()), error->message);
}

FlatpakInstallation *FlatpakSource::installation() const
{
    return m_installation;
}

FlatpakRemote *FlatpakSource::remote() const
{
    return m_remote;
}

QDir FlatpakSource::appStreamDir() const
{
    g_autoptr(GFile) dir = flatpak_remote_get_appstream_dir(m_remote, nullptr);
    if (!dir) {
        qCWarning(lcFlatpakBackend, "No AppStream directory for \"%s\"",
                  qPrintable(name()));
        return QDir();
    }

    return QDir(QString::fromUtf8(g_file_get_path(dir)));
}
