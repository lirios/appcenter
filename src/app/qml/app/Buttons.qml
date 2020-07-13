// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Core 1.0 as FluidCore
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

RowLayout {
    Layout.fillWidth: true

    spacing: FluidControls.Units.smallSpacing

    Button {
        Layout.alignment: Qt.AlignVCenter

        ToolTip.text: qsTr("Get this resource on your device")
        ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

        text: qsTr("Get")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: FluidCore.Device.hoverEnabled
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

    Button {
        Layout.alignment: Qt.AlignVCenter

        ToolTip.text: qsTr("Remove this resource from your device")
        ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

        text: qsTr("Remove")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: FluidCore.Device.hoverEnabled
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

    Button {
        Layout.alignment: Qt.AlignVCenter

        ToolTip.text: qsTr("Update this resource")
        ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

        text: qsTr("Update")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: FluidCore.Device.hoverEnabled
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

    Button {
        Layout.alignment: Qt.AlignVCenter

        ToolTip.text: qsTr("Open this resource up")
        ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
        ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

        text: qsTr("Open")
        flat: true
        enabled: !transactionIndicator.visible
        hoverEnabled: FluidCore.Device.hoverEnabled
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
