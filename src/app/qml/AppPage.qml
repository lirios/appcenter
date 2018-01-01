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

import QtQuick 2.4
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import Liri.Core 1.0 as LiriCore
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Page {
    id: appPage

    property var app

    title: app.name
    padding: 16

    FluidControls.OverlayView {
        id: screenshotOverlay

        property alias source: screenshot.source

        width: Math.min(appPage.width * 0.9, screenshot.sourceSize.width)
        height: Math.min(appPage.height * 0.9, screenshot.sourceSize.height)

        Image {
            id: screenshot
            anchors.fill: parent

            MouseArea {
                anchors.fill: parent
                onClicked: screenshotOverlay.close()
            }
        }

        BusyIndicator {
            anchors.centerIn: parent
            visible: screenshot.status != Image.Ready
        }

        function show(url) {
            source = url;
            open();
        }
    }

    Flickable {
        id: scrollView
        anchors.fill: parent

        contentHeight: column.height + column.anchors.margins * 2

        ScrollBar.vertical: ScrollBar {}

        ColumnLayout {
            id: column
            anchors.fill: parent

            spacing: 16

            RowLayout {
                Layout.fillWidth: true

                spacing: 16

                FluidControls.Icon {
                    id: icon

                    size: 64
                    name: app.iconName
                    visible: !image.visible
                }

                Image {
                    id: image

                    width: 64
                    height: 64
                    source: app.iconUrl
                    visible: app.iconUrl.toString() !== ""
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignVCenter

                    FluidControls.HeadlineLabel {
                        text: app.name
                    }

                    FluidControls.SubheadingLabel {
                        text: app.summary
                        color: Material.secondaryTextColor
                    }
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    text: qsTr("Install")
                    visible: app.state === AppCenter.SoftwareResource.NotInstalledState
                    onClicked: {
                        if (!app.install())
                            console.log("Something went wrong!")
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    text: qsTr("Remove")
                    visible: app.state === AppCenter.SoftwareResource.InstalledState ||
                             app.state === AppCenter.SoftwareResource.UpgradableState
                    onClicked: {
                        if (!app.uninstall())
                            console.log("Something went wrong!")
                    }

                    Material.foreground: Material.color(Material.Red, Material.Shade100)
                    Material.background: Material.color(Material.Red, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    visible: app.state === AppCenter.SoftwareResource.UpgradableState
                    text: qsTr("Update")
                    onClicked: {
                        if (!app.update())
                            console.log("Something went wrong!")
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    visible: app.state === AppCenter.SoftwareResource.InstalledState ||
                             app.state === AppCenter.SoftwareResource.UpgradableState
                    text: qsTr("Launch")
                    onClicked: {
                        if (!app.launch())
                            console.log("Something went wrong!")
                    }

                    Material.foreground: Material.color(Material.Green, Material.Shade100)
                    Material.background: Material.color(Material.Green, Material.Shade500)
                }
            }

            Label {
                Layout.fillWidth: true

                text: app.description
                wrapMode: Label.WordWrap
                visible: text !== ""
            }

            RowLayout {
                Layout.fillWidth: true

                spacing: 16

                Button {
                    text: qsTr("Website")
                    visible: app.homepageUrl.toString() !== ""
                    onClicked: Qt.openUrlExternally(app.homepageUrl)
                }
            }

            ListView {
                ScrollBar.horizontal: ScrollBar {}

                Layout.alignment: Qt.AlignTop
                Layout.preferredHeight: 120
                Layout.fillWidth: true

                clip: true
                orientation: ListView.Horizontal
                spacing: 8
                model: AppCenter.ScreenshotsModel {
                    id: screenshotsModel
                    app: appPage.app
                }
                delegate: Image {
                    id: thumbnail

                    source: model.thumbnailUrl

                    width: height * (model.thumbnailSize.width / model.thumbnailSize.height)
                    height: 120
                    visible: status !== Image.Error

                    BusyIndicator {
                        anchors.centerIn: parent
                        visible: thumbnail.status == Image.Loading
                    }

                    FluidControls.Ripple {
                        anchors.fill: parent
                        onClicked: screenshotOverlay.show(model.screenshotUrl)
                    }
                }
            }

            FluidControls.TitleLabel {
                text: qsTr("Details")
            }

            RowLayout {
                Layout.fillWidth: true

                spacing: 16

                GridLayout {
                    rows: 5
                    columns: 2
                    columnSpacing: 8

                    // Localization

                    FluidControls.Icon {
                        name: "action/check_circle"
                        color: app.localized ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Localized in your language")
                    }

                    // Documentation

                    FluidControls.Icon {
                        name: "action/check_circle"
                        color: false ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Documentation")
                    }

                    // Releases

                    FluidControls.Icon {
                        name: "action/check_circle"
                        color: false ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Release activity")
                    }

                    // System integration

                    FluidControls.Icon {
                        name: "action/check_circle"
                        color: false ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("System integration")
                    }

                    // Sandboxed

                    FluidControls.Icon {
                        name: "action/check_circle"
                        color: false ? Material.color(Material.Green) : Material.secondaryTextColor
                    }

                    Label {
                        text: qsTr("Sandboxed")
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                GridLayout {
                    rows: 6
                    columns: 2
                    columnSpacing: 8

                    // Version

                    Label {
                        text: qsTr("Version")
                    }

                    Label {
                        text: app.version
                    }

                    // Updated

                    Label {
                        text: qsTr("Updated")
                    }

                    Label {
                        text: qsTr("Unknown")
                    }

                    // Category

                    Label {
                        text: qsTr("Category")
                    }

                    Label {
                        text: app.category ? app.category : qsTr("Unknown")
                    }

                    // License

                    Label {
                        text: qsTr("License")
                    }

                    Label {
                        text: app.license ? app.license : qsTr("Unknown")
                        onLinkActivated: Qt.openUrlExternally(link)
                    }

                    // Source

                    Label {
                        text: qsTr("Source")
                    }

                    Label {
                        text: app.origin ? app.origin : qsTr("Unknown")
                    }

                    // Size

                    Label {
                        text: app.state === AppCenter.SoftwareResource.InstalledState
                              ? qsTr("Installed size") : qsTr("Download size")
                    }

                    Label {
                        text: LiriCore.Formatter.formatByteSize(app.state === AppCenter.SoftwareResource.InstalledState ? app.installedSize : app.downloadSize)
                    }
                }
            }

            FluidControls.TitleLabel {
                text: qsTr("Reviews")
            }

            Label {
                text: qsTr("Sorry, but reviews are not implemented yet.")
                color: Material.secondaryTextColor
            }
        }
    }
}
