/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2017 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

import QtQuick 2.0
import QtQuick.Controls 2.2
import Fluid.Controls 1.0 as FluidControls

FluidControls.Tab {
    title: qsTr("Sources")

    Dialog {
        id: editDialog

        property var source: null

        title: qsTr("Edit %1").arg(source ? source.name : "")

        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true

        Label {
            text: "Not implemented yet"
        }

        function show(source) {
            editDialog.source = source;
            editDialog.open();
        }
    }

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
                        icon.source: FluidControls.Utils.iconUrl("content/create")
                        onClicked: editDialog.show(model.source)

                        ToolTip.text: qsTr("Edit")
                        ToolTip.visible: hovered
                    }

                    ToolButton {
                        icon.source: FluidControls.Utils.iconUrl("content/remove")
                        onClicked: softwareManager.removeSource(model.source)

                        ToolTip.text: qsTr("Remove")
                        ToolTip.visible: hovered
                    }
                }
                text: qsTr("%1 (%2)").arg(model.title).arg(model.name)
                subText: model.url
            }
        }
    }
}
