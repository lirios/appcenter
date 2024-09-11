// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid.Core as FluidCore
import Fluid as Fluid
import "../ratings" as Ratings

Fluid.Page {
    readonly property real reviewRating: model.rating
    property bool drawBorder: true
    property bool isLastItem: ListView.view && index + 1 === ListView.view.count

    background: Rectangle {
        radius: 2
        color: "transparent"
        border.color: drawBorder ? Material.dividerColor : "transparent"
    }

    leftPadding: Fluid.Units.smallSpacing * 2
    rightPadding: Fluid.Units.smallSpacing * 2
    topPadding: Fluid.Units.smallSpacing * 2
    bottomPadding: Fluid.Units.smallSpacing

    Fluid.AlertDialog {
        id: reportDialog

        parent: appPage

        anchors.centerIn: parent

        width: 400

        title: qsTr("Report review?")
        text: qsTr("Reviews can be reported for abusive, rude or discriminatory behavior.\n" +
                   "Once reported, a review is hidden until an administrator has verified it.")
        standardButtons: Dialog.Yes | Dialog.No

        onAccepted: {
            model.review.report();
        }
    }

    Fluid.AlertDialog {
        id: removeDialog

        parent: appPage

        anchors.centerIn: parent

        width: 400

        title: qsTr("Remove review?")
        text: qsTr("You can remove your review if you are not satisfied with it.\n" +
                   "Reviews can be submitted again later.")
        standardButtons: Dialog.Yes | Dialog.No

        onAccepted: {
            model.review.remove();
        }
    }

    ColumnLayout {
        anchors.fill: parent

        Fluid.TitleLabel {
            Layout.fillWidth: true

            text: model.summary
            elide: Text.ElideRight
        }

        RowLayout {
            Layout.preferredHeight: Fluid.Units.smallSpacing * 4

            spacing: Fluid.Units.smallSpacing

            Row {
                Layout.alignment: Qt.AlignVCenter

                Repeater {
                    model: 5

                    Ratings.RatingStar {
                        Layout.alignment: Qt.AlignVCenter

                        star: index + 1
                        rating: (reviewRating * 5) / 100
                    }
                }
            }

            Fluid.BodyLabel {
                Layout.fillWidth: true
                Layout.alignment: Qt.AlignVCenter

                text: model.reviewerName
                    ? qsTr("%1 by %2").arg(model.creationDate.toLocaleDateString()).arg(model.reviewerName)
                    : model.creationDate.toLocaleDateString()
                elide: Text.ElideRight
                color: Material.secondaryTextColor
            }
        }

        Fluid.BodyLabel {
            Layout.fillWidth: true
            Layout.fillHeight: true

            text: model.description
            wrapMode: Text.Wrap
            elide: Text.ElideRight
        }

        Fluid.Label {
            text: qsTr("%n people found this review helpful", "", model.karmaUp)
            color: Material.secondaryTextColor
            visible: model.karmaUp > 0
        }

        RowLayout {
            Layout.maximumHeight: Fluid.Units.smallSpacing * 6

            Fluid.Label {
                Layout.alignment: Qt.AlignVCenter

                text: qsTr("Was this review helpful?")
            }

            Fluid.ToolButton {
                Layout.alignment: Qt.AlignVCenter

                Fluid.ToolTip.text: qsTr("Up vote")
                Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
                Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

                icon.source: Fluid.Utils.iconUrl("action/thumb_up")
                icon.width: 18
                icon.height: 18

                flat: true
                text: qsTr("Yes")
                hoverEnabled: FluidCore.Device.hoverEnabled
                onClicked: {
                    model.review.upVote();
                }
            }

            Fluid.ToolButton {
                Layout.alignment: Qt.AlignVCenter

                Fluid.ToolTip.text: qsTr("Down vote")
                Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
                Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

                icon.source: Fluid.Utils.iconUrl("action/thumb_down")
                icon.width: 18
                icon.height: 18

                flat: true
                text: qsTr("No")
                hoverEnabled: FluidCore.Device.hoverEnabled
                onClicked: {
                    model.review.downVote();
                }
            }

            Item {
                Layout.fillWidth: true
            }

            Fluid.Button {
                Layout.alignment: Qt.AlignVCenter

                Fluid.ToolTip.text: qsTr("Report this review")
                Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
                Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

                flat: true
                text: qsTr("Report...")
                hoverEnabled: FluidCore.Device.hoverEnabled
                visible: !model.selfMade
                onClicked: {
                    reportDialog.open();
                }
            }

            Fluid.Button {
                Layout.alignment: Qt.AlignVCenter

                Fluid.ToolTip.text: qsTr("Remove this review")
                Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
                Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

                flat: true
                text: qsTr("Remove...")
                hoverEnabled: FluidCore.Device.hoverEnabled
                visible: model.selfMade
                onClicked: {
                    removeDialog.open();
                }
            }
        }

        Fluid.ThinDivider {
            Layout.fillWidth: true

            visible: !drawBorder && !isLastItem
        }
    }
}
