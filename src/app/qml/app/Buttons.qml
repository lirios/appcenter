// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import Liri.AppCenter as AppCenter

RowLayout {
    Layout.fillWidth: true

    spacing: Fluid.Units.smallSpacing

    Fluid.Button {
        Layout.alignment: Qt.AlignVCenter

        ToolTip.text: qsTr("Get this resource on your device")
        ToolTip.delay: Fluid.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        ToolTip.visible: Fluid.Device.isMobile ? pressed : hovered

        text: qsTr("Get")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: Fluid.Device.hoverEnabled
        visible: app.state === AppCenter.SoftwareResource.NotInstalledState
        onClicked: {
            var transaction = app.install();
            if (!transaction) {
                console.error("Installation of", app.name, "has failed: null transaction");
                return;
            }
            transactionIndicator.transaction = transaction;
            transaction.run();
        }

        Material.foreground: Material.color(Material.Green, Material.Shade100)
        Material.background: Material.color(Material.Green, Material.Shade500)
    }

    Fluid.Button {
        Layout.alignment: Qt.AlignVCenter

        Fluid.ToolTip.text: qsTr("Remove this resource from your device")
        Fluid.ToolTip.delay: Fluid.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        Fluid.ToolTip.visible: Fluid.Device.isMobile ? pressed : hovered

        text: qsTr("Remove")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: Fluid.Device.hoverEnabled
        visible: app.state === AppCenter.SoftwareResource.InstalledState ||
                 app.state === AppCenter.SoftwareResource.UpgradableState
        onClicked: {
            var transaction = app.uninstall();
            if (!transaction) {
                console.error("Uninstallation of", app.name, "has failed: null transaction");
                return;
            }
            transactionIndicator.transaction = transaction;
            transaction.run();
        }

        Material.foreground: Material.color(Material.Green, Material.Shade500)
    }

    Fluid.Button {
        Layout.alignment: Qt.AlignVCenter

        Fluid.ToolTip.text: qsTr("Update this resource")
        Fluid.ToolTip.delay: Fluid.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        Fluid.ToolTip.visible: Fluid.Device.isMobile ? pressed : hovered

        text: qsTr("Update")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: Fluid.Device.hoverEnabled
        visible: app.state === AppCenter.SoftwareResource.UpgradableState
        onClicked: {
            var transaction = app.update();
            if (!transaction) {
                console.error("Update of", app.name, "has failed: null transaction");
                return;
            }
            transactionIndicator.transaction = transaction;
            transaction.run();
        }

        Material.foreground: Material.color(Material.Green, Material.Shade100)
        Material.background: Material.color(Material.Green, Material.Shade500)
    }

    Fluid.Button {
        Layout.alignment: Qt.AlignVCenter

        Fluid.ToolTip.text: qsTr("Open this resource up")
        Fluid.ToolTip.delay: Fluid.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        Fluid.ToolTip.visible: Fluid.Device.isMobile ? pressed : hovered

        text: qsTr("Open")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: Fluid.Device.hoverEnabled
        visible: app.state === AppCenter.SoftwareResource.InstalledState
        onClicked: {
            if (!app.launch()) {
                console.error("Execution of", app.name, "has failed")
                return;
            }
        }

        Material.foreground: Material.color(Material.Green, Material.Shade100)
        Material.background: Material.color(Material.Green, Material.Shade500)
    }
}
