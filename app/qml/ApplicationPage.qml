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
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Page {
    id: appPage

    property var app
    property int selectedImageIndex

    title: app.name

    Flickable {
        id: scrollView
        anchors.fill: parent

        contentHeight: column.height + column.anchors.margins * 2

        ScrollBar.vertical: ScrollBar {}

        ColumnLayout {
            id: column
            width: Math.min(800, scrollView.width - 2 * anchors.margins)

            spacing: 16

            anchors {
                horizontalCenter: parent.horizontalCenter
                top: parent.top
                margins: 16
            }

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

                Item {
                    Layout.fillWidth: true
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

            RowLayout {
                Layout.fillWidth: true

                spacing: 16

                Image {
                    Layout.preferredWidth: 630
                    Layout.preferredHeight: width * sourceSize.height/sourceSize.width
                    Layout.alignment: Qt.AlignTop

                    source: screenshotsModel.screenshotUrlAt(selectedImageIndex)
                    fillMode: Image.Pad
                    clip: true
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignTop
                    spacing: 8

                    Repeater {
                        model: AppCenter.ScreenshotsModel {
                            id: screenshotsModel
                            app: appPage.app
                        }
                        delegate: Image {
                            source: model.thumbnailUrl

                            Layout.preferredWidth: 120
                            Layout.preferredHeight: width * sourceSize.height/sourceSize.width

                            FluidControls.Ripple {
                                anchors.fill: parent
                                onClicked: selectedImageIndex = index
                            }
                        }
                    }
                }
            }
        }
    }
}
