// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.ApplicationWindow {
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

    FluidControls.SnackBar {
        id: snackBar

        duration: 5000
    }

    initialPage: FluidControls.TabbedPage {
        title: qsTr("App Center")

        actions: [
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("content/add")
                text: qsTr("Add source")
                toolTip: qsTr("Add a new source")
                onTriggered: addSourceDialog.open()
            },
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("navigation/refresh")
                text: qsTr("Check for updates")
                toolTip: qsTr("Check for updates")
                onTriggered: softwareManager.checkForUpdates()
            },
            FluidControls.Action {
                icon.source: FluidControls.Utils.iconUrl("action/search")
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
