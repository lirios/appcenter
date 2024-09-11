// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import Fluid as Fluid

Fluid.Tab {
    title: qsTr("Sources")

    Fluid.ScrollView {
        anchors.fill: parent

        ListView {
            model: softwareManager.sourcesModel
            section.delegate: Fluid.Subheader {
                text: section
            }
            section.property: "section"
            delegate: Fluid.ListItem {
                leftItem: CheckBox {
                    anchors.verticalCenter: parent.verticalCenter
                    checked: model.enabled
                    onCheckedChanged: model.source.enabled = checked
                }
                rightItem: Row {
                    anchors.verticalCenter: parent.verticalCenter

                    Fluid.ToolButton {
                        icon.source: Fluid.Utils.iconUrl("content/remove")
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
