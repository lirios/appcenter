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

#ifndef LIRI_FLATPAKPLUGIN_H
#define LIRI_FLATPAKPLUGIN_H

#include <LiriAppCenter/BackendPlugin>

#include "flatpakbackend.h"

class FlatpakPlugin : public Liri::AppCenter::BackendPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID BackendPlugin_iid FILE "flatpak.json")
    Q_INTERFACES(Liri::AppCenter::BackendPlugin)
public:
    explicit FlatpakPlugin(QObject *parent = nullptr);

    Liri::AppCenter::Backend *create() override;
};

#endif // LIRI_FLATPAKPLUGIN_H
