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
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import QtQuick.Layouts 1.0
import Fluid.Controls 1.0
import Fluid.Material 1.0 as FluidMaterial
import Liri.Software 0.1 as Software

Page {
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

                Icon {
                    id: image
                    Layout.fillHeight: true
                    Layout.preferredWidth: height

                    //icon: app.icon
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignVCenter

                    HeadlineLabel {
                        text: app.name
                    }

                    SubheadingLabel {
                        text: app.summary
                        color: Material.secondaryTextColor
                    }
                }

                Item {
                    Layout.fillWidth: true
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    text: app.state === Software.Application.Installed  ? "Uninstall" : "Install"

                    Material.elevation: 1
                    Material.background: app.state === Software.Application.Installed
                                         ? Material.color(Material.Red, Material.Shade500) : Material.primaryColor
                }

                Button {
                    Layout.alignment: Qt.AlignVCenter

                    visible: app.state === Software.Application.Installed
                    text: "Open"
                    // backgroundColor: Material.color(Material.Green, Material.Shade500)
                    onClicked: {
                        if (!app.launch())
                            console.log("Something went wrong!")
                    }

                    Material.elevation: 1
                }
            }

            RowLayout {
                Layout.fillWidth: true

                spacing: 16

                Image {
                    Layout.fillWidth: true
                    Layout.preferredHeight: width * sourceSize.height/sourceSize.width
                    Layout.alignment: Qt.AlignTop

                    source: app.screenshots.get(selectedImageIndex).url
                }

                ColumnLayout {
                    Layout.alignment: Qt.AlignTop
                    spacing: 8

                    Repeater {
                        model: app.screenshots
                        delegate: Image {
                            source: edit.url

                            Layout.preferredWidth: 120
                            Layout.preferredHeight: width * sourceSize.height/sourceSize.width

                            FluidMaterial.Ripple {
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
