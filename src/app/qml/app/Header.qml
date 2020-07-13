// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls

ColumnLayout {
    property alias iconSize: icon.iconSize

    RowLayout {
        Layout.fillWidth: true
        Layout.alignment: Qt.AlignVCenter

        spacing: FluidControls.Units.smallSpacing

        Icon {
            id: icon

            Layout.alignment: Qt.AlignVCenter

            iconSize: 256
            iconName: app.iconName
            iconUrl: app.iconUrl
        }

        ColumnLayout {
            Layout.alignment: Qt.AlignVCenter
            Layout.fillWidth: true

            ColumnLayout {
                Layout.fillWidth: true

                spacing: FluidControls.Units.smallSpacing

                FluidControls.HeadlineLabel {
                    Layout.fillWidth: true

                    text: app.name
                    elide: Text.ElideRight
                }

                FluidControls.SubheadingLabel {
                    Layout.fillWidth: true
                    Layout.fillHeight: true

                    text: app.summary
                    color: Material.secondaryTextColor
                    wrapMode: Label.WordWrap
                    elide: Text.ElideRight
                }

                // FIXME: Finish this once we have the information to show
                /*
                    FluidControls.BodyLabel {
                        text: "<a href=\"www\">Developer</a>"
                        level: 2
                    }
                */

                Buttons {
                    Layout.alignment: Qt.AlignVCenter | Qt.AlignLeft
                }
            }

            FluidControls.ThinDivider {
                Layout.fillWidth: true
            }

            HeaderRatings {
                Layout.fillWidth: true
            }
        }
    }
}
