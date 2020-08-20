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

#ifndef LIRI_FLATPAKSOURCE_H
#define LIRI_FLATPAKSOURCE_H

#include <QDir>

#include <LiriAppCenter/SoftwareSource>

#include "flatpakpluginbase.h"

class FlatpakSource : public Liri::AppCenter::SoftwareSource
{
    Q_OBJECT
public:
    explicit FlatpakSource(Liri::AppCenter::Backend *backend,
                           FlatpakInstallation *installation,
                           FlatpakRemote *remote,
                           QObject *parent = nullptr);
    ~FlatpakSource();

    QString name() const override;

    QString title() const override;
    void setTitle(const QString &value) override;

    QString comment() const override;
    void setComment(const QString &value) override;

    QString description() const override;
    void setDescription(const QString &value) override;

    QString section() const override;

    QString label() const override;

    bool isEnabled() const override;
    void setEnabled(bool value) override;

    bool gpgVerify() const override;
    void setGpgVerify(bool value) override;

    QUrl url() const override;
    void setUrl(const QUrl &value) override;

    QUrl iconUrl() const override;
    void setIconUrl(const QUrl &value) override;

    int priority() const override;
    void setPriority(int value) override;

    FlatpakInstallation *installation() const;
    FlatpakRemote *remote() const;

    QDir appStreamDir() const;

private:
    FlatpakInstallation *m_installation = nullptr;
    FlatpakRemote *m_remote = nullptr;
    GCancellable *m_cancellable = nullptr;
    bool m_isUser = false;
};

#endif // LIRI_FLATPAKSOURCE_H
