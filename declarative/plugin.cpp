/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Michael Spencer <sonrisesoftware@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

#include "plugin.h"

#include "softwaremanager.h"
#include "source.h"
#include "application.h"
#include "screenshot.h"

void SoftwarePlugin::registerTypes(const char *uri)
{
    // @uri Papyros.Software
    Q_ASSERT(uri == QLatin1String("Papyros.Software"));

    qmlRegisterType<SoftwareManager>(uri, 0, 1, "SoftwareManager");
    qmlRegisterUncreatableType<SoftwareSource>(uri, 0, 1, "SoftwareSource", "A remote for xdg-app");
    qmlRegisterUncreatableType<Application>(uri, 0, 1, "Application", "An application for xdg-app");
    qmlRegisterUncreatableType<Screenshot>(uri, 0, 1, "Screenshot", "An screenshot for xdg-app");
}
