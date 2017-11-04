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

#include <QQmlComponent>
#include <QQmlExtensionPlugin>

#include <LiriAppCenter/ResourcesModel>
#include <LiriAppCenter/ScreenshotsModel>
#include <LiriAppCenter/SoftwareManager>
#include <LiriAppCenter/SoftwareResource>
#include <LiriAppCenter/SoftwareSource>
#include <LiriAppCenter/SourcesModel>

#include "filteredresourcesmodel.h"

using namespace Liri::AppCenter;

class LiriAppCenterPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.qt-project.Qt.QQmlExtensionInterface")
public:
    void registerTypes(const char *uri)
    {
        // @uri Liri.AppCenter
        Q_ASSERT(QLatin1String(uri) == QLatin1String("Liri.AppCenter"));

        qmlRegisterType<FilteredResourcesModel>(uri, 1, 0, "FilteredResourcesModel");
        qmlRegisterType<ScreenshotsModel>(uri, 1, 0, "ScreenshotsModel");
        qmlRegisterType<SoftwareManager>(uri, 1, 0, "SoftwareManager");
        qmlRegisterType<SourcesModel>(uri, 1, 0, "SourcesModel");

        qmlRegisterUncreatableType<ResourcesModel>(uri, 1, 0, "ResourcesModel", QLatin1String("Unable to instantiate ResourcesModel"));
        qmlRegisterUncreatableType<SoftwareResource>(uri, 1, 0, "SoftwareResource", QLatin1String("Unable to instantiate SoftwareResource"));
        qmlRegisterUncreatableType<SoftwareSource>(uri, 1, 0, "SoftwareSource", QLatin1String("Unable to instantiate SoftwareSource"));

        qmlProtectModule(uri, 1);
    }
};

#include "plugin.moc"
