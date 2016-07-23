/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2016 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Hawaii.Settings 1.0
import Papyros.Software 0.1

FluidWindow {
    id: demo

    title: qsTr("App Center")

    width: 800
    height: 600
    visible: true

    Material.primary: Material.Blue
    Material.accent: Material.Blue

    Action {
        id: searchAction

        iconName: "action/search"
        text: "Search"
    }

    initialPage: Page {
        title: qsTr("App Center")

        actions: [searchAction]

        Pane {
            id: listPane

            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }

            Material.background: "white"
            Material.elevation: 1

            width: 200
            padding: 0
            z: 2

            ListView {
                anchors.fill: parent

                model: ListModel {
                    ListElement { text: qsTr("All Apps"); iconName: "action/shop"; url: "AllAppsPage.qml" }
                    ListElement { text: qsTr("Installed Apps"); iconName: "file/file_download"; url: "InstalledAppsPage.qml" }
                    ListElement { text: qsTr("Settings"); iconName: "action/settings"; url: "SettingsPage.qml" }
                }
                delegate: ListItem {
                    text: model.text
                    iconName: model.iconName
                    onClicked: pageStack.push(Qt.resolvedUrl(url))
                }

                ScrollBar.vertical: ScrollBar {}
            }
        }

        ListView {
            anchors {
                left: listPane.right
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }

            model: software.availableApps
            delegate: ListItem {
                text: edit.name
                subText: edit.summary
                valueText: edit.branch
                iconName: edit.icon
                onClicked: pageStack.push(Qt.resolvedUrl("ApplicationPage.qml"), {app: edit})
            }
        }
    }

    SoftwareManager {
        id: software
    }
}
