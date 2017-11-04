/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

import QtQuick 2.0
import QtQuick.Controls 2.0
import Fluid.Core 1.0 as FluidCore
import Fluid.Controls 1.0 as FluidControls
import Liri.Core 1.0 as LiriCore
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Tab {
    title: qsTr("Installed Apps")

    ListView {
        anchors.fill: parent

        model: AppCenter.FilteredResourcesModel {
            sourceModel: softwareManager.resourcesModel
            filter: AppCenter.FilteredResourcesModel.InstalledApps
        }
        delegate: FluidControls.ListItem {
            text: model.resource.name
            subText: model.resource.summary
            valueText: LiriCore.Formatter.formatByteSize(model.resource.installedSize)
            onClicked: pageStack.push(Qt.resolvedUrl("ApplicationPage.qml"), {app: model.resource})
        }
    }
}
