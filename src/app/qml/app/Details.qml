// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Liri.Core 1.0 as LiriCore
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

ColumnLayout {
    Layout.fillWidth: true

    FluidControls.HeadlineLabel {
        Layout.fillWidth: true

        text: qsTr("Details")
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter

        spacing: FluidControls.Units.smallSpacing * 2

        GridLayout {
            id: kudosGrid

            rows: 5
            columns: 2
            rowSpacing: FluidControls.Units.smallSpacing
            columnSpacing: FluidControls.Units.smallSpacing

            // Localization

            FluidControls.Icon {
                source: FluidControls.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.MyLanguageKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Label {
                text: qsTr("Localized in your language")
            }

            // Documentation

            FluidControls.Icon {
                source: FluidControls.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.InstallsUserDocsKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Label {
                text: qsTr("Documentation")
            }

            // Releases

            FluidControls.Icon {
                source: FluidControls.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.RecentReleaseKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Label {
                text: qsTr("Release activity")
            }

            // System integration

            FluidControls.Icon {
                source: FluidControls.Utils.iconUrl("action/check_circle")
                color: (app.hasKudo(AppCenter.SoftwareResource.SearchProviderKudo) ||
                        app.hasKudo(AppCenter.SoftwareResource.UsesNotificationsKudo) ||
                        app.hasKudo(AppCenter.SoftwareResource.HighContrastKudo))
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Label {
                text: qsTr("System integration")
            }

            // Sandboxed

            FluidControls.Icon {
                source: FluidControls.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.SandboxedKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Label {
                text: qsTr("Sandboxed")
            }
        }

        GridLayout {
            id: infoGrid

            rows: 6
            columns: 2
            rowSpacing: FluidControls.Units.smallSpacing
            columnSpacing: FluidControls.Units.smallSpacing

            // Version

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Version:")
                font.bold: true
            }

            Label {
                text: app.installedVersion
                color: Material.secondaryTextColor
            }

            // Updated

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Updated:")
                font.bold: true
            }

            Label {
                text: qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // Category

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Category:")
                font.bold: true
            }

            Label {
                text: app.category ? app.category : qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // License

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("License:")
                font.bold: true
            }

            Label {
                text: app.license ? app.license : qsTr("Unknown")
                color: Material.secondaryTextColor
                onLinkActivated: Qt.openUrlExternally(link)
            }

            // Source

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Source:")
                font.bold: true
            }

            Label {
                text: app.origin ? app.origin : qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // Size

            Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Size:")
                font.bold: true
            }

            Label {
                text: LiriCore.Formatter.formatByteSize(app.size)
                color: Material.secondaryTextColor
            }
        }
    }
}
