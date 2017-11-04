/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

#ifndef FLATPAKSOURCE_H
#define FLATPAKSOURCE_H

#include <QDir>

#include <LiriAppCenter/SoftwareSource>

#include "flatpakpluginbase.h"

class FlatpakSource : public Liri::AppCenter::SoftwareSource
{
    Q_OBJECT
public:
    explicit FlatpakSource(FlatpakInstallation *installation,
                           FlatpakRemote *remote,
                           QObject *parent = nullptr);
    ~FlatpakSource();

    FlatpakInstallation *installation() const;
    void setInstallation(FlatpakInstallation *installation);

    FlatpakRemote *remote() const;
    void setRemote(FlatpakRemote *remote);

    QDir appStreamDir() const;

private:
    FlatpakInstallation *m_installation = nullptr;
    FlatpakRemote *m_remote = nullptr;
};

#endif // FLATPAKSOURCE_H
