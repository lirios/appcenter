// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Controls.Material
import Fluid as Fluid
import Liri.AppCenter as AppCenter

Fluid.ApplicationWindow {
    title: qsTr("App Center")

    width: 1024
    height: 768
    minimumWidth: 600
    minimumHeight: 600
    visible: true

    Material.primary: Material.color(Material.Green, Material.Shade800)
    Material.accent: Material.color(Material.Green, Material.Shade600)

    AppCenter.SoftwareManager {
        id: softwareManager

        onSourceAddFailed: snackBar.open(errorMessage)

        Component.onCompleted: softwareManager.initialize()
    }

    AddSourceDialog {
        id: addSourceDialog
        onFailed: snackBar.open(errorMessage)
    }

    Fluid.SnackBar {
        id: snackBar

        duration: 5000
    }

    initialPage: Fluid.TabbedPage {
        title: qsTr("App Center")

        actions: [
            Fluid.Action {
                icon.source: Fluid.Utils.iconUrl("content/add")
                text: qsTr("Add source")
                toolTip: qsTr("Add a new source")
                onTriggered: addSourceDialog.open()
            },
            Fluid.Action {
                icon.source: Fluid.Utils.iconUrl("navigation/refresh")
                text: qsTr("Check for updates")
                toolTip: qsTr("Check for updates")
                onTriggered: softwareManager.checkForUpdates()
            },
            Fluid.Action {
                icon.source: Fluid.Utils.iconUrl("action/search")
                text: qsTr("Search")
                toolTip: qsTr("Search apps")
            }
        ]

        AllAppsTab {}
        InstalledAppsTab {}
        UpdatesTab {}
        SourcesTab {}
    }
}
