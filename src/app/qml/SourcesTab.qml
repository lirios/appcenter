// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.2
import Fluid.Controls 1.0 as FluidControls

FluidControls.Tab {
    title: qsTr("Sources")

    ScrollView {
        anchors.fill: parent

        ListView {
            model: softwareManager.sourcesModel
            section.delegate: FluidControls.Subheader {
                text: section
            }
            section.property: "section"
            delegate: FluidControls.ListItem {
                leftItem: CheckBox {
                    anchors.verticalCenter: parent.verticalCenter
                    checked: model.enabled
                    onCheckedChanged: model.source.enabled = checked
                }
                rightItem: Row {
                    anchors.verticalCenter: parent.verticalCenter

                    ToolButton {
                        icon.source: FluidControls.Utils.iconUrl("content/remove")
                        onClicked: {
                            softwareManager.removeSource(model.source);
                        }

                        ToolTip.text: qsTr("Remove")
                        ToolTip.visible: hovered
                    }
                }
                text: model.label
                subText: model.hostname
            }
        }
    }
}
