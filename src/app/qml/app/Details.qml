// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Liri.Core as LiriCore
import Fluid as Fluid
import Liri.AppCenter as AppCenter

ColumnLayout {
    Layout.fillWidth: true

    Fluid.HeadlineLabel {
        Layout.fillWidth: true

        text: qsTr("Details")
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter

        spacing: Fluid.Units.smallSpacing * 2

        GridLayout {
            id: kudosGrid

            rows: 5
            columns: 2
            rowSpacing: Fluid.Units.smallSpacing
            columnSpacing: Fluid.Units.smallSpacing

            // Localization

            Fluid.Icon {
                source: Fluid.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.MyLanguageKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Fluid.Label {
                text: qsTr("Localized in your language")
            }

            // Documentation

            Fluid.Icon {
                source: Fluid.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.InstallsUserDocsKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Fluid.Label {
                text: qsTr("Documentation")
            }

            // Releases

            Fluid.Icon {
                source: Fluid.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.RecentReleaseKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Fluid.Label {
                text: qsTr("Release activity")
            }

            // System integration

            Fluid.Icon {
                source: Fluid.Utils.iconUrl("action/check_circle")
                color: (app.hasKudo(AppCenter.SoftwareResource.SearchProviderKudo) ||
                        app.hasKudo(AppCenter.SoftwareResource.UsesNotificationsKudo) ||
                        app.hasKudo(AppCenter.SoftwareResource.HighContrastKudo))
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Fluid.Label {
                text: qsTr("System integration")
            }

            // Sandboxed

            Fluid.Icon {
                source: Fluid.Utils.iconUrl("action/check_circle")
                color: app.hasKudo(AppCenter.SoftwareResource.SandboxedKudo)
                       ? Material.color(Material.Green) : Material.secondaryTextColor
            }

            Fluid.Label {
                text: qsTr("Sandboxed")
            }
        }

        GridLayout {
            id: infoGrid

            rows: 6
            columns: 2
            rowSpacing: Fluid.Units.smallSpacing
            columnSpacing: Fluid.Units.smallSpacing

            // Version

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Version:")
                font.bold: true
            }

            Fluid.Label {
                text: app.installedVersion
                color: Material.secondaryTextColor
            }

            // Updated

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Updated:")
                font.bold: true
            }

            Fluid.Label {
                text: qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // Category

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Category:")
                font.bold: true
            }

            Fluid.Label {
                text: app.category ? app.category : qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // License

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("License:")
                font.bold: true
            }

            Fluid.Label {
                text: app.license ? app.license : qsTr("Unknown")
                color: Material.secondaryTextColor
                onLinkActivated: Qt.openUrlExternally(link)
            }

            // Source

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Source:")
                font.bold: true
            }

            Fluid.Label {
                text: app.defaultSource ? app.defaultSource.name : qsTr("Unknown")
                color: Material.secondaryTextColor
            }

            // Size

            Fluid.Label {
                Layout.alignment: Qt.AlignRight

                text: qsTr("Size:")
                font.bold: true
            }

            Fluid.Label {
                text: LiriCore.Formatter.formatByteSize(app.size)
                color: Material.secondaryTextColor
            }
        }
    }
}
