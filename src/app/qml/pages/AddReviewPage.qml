// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import Liri.AppCenter as AppCenter

Fluid.Page {
    property var resource
    property int rating: 0
    property int hoverRating: 0
    property bool hovering: false
    property bool submitted: false

    title: qsTr("Write a review for %1").arg(resource.name)

    padding: Fluid.Units.smallSpacing * 2

    actions: [
        Fluid.Action {
            icon.source: Fluid.Utils.iconUrl("content/save")

            toolTip: qsTr("Submit this review")
            text: qsTr("Submit")
            enabled: !submitted

            onTriggered: {
                resource.submitReview(summaryField.text, descriptionField.text, rating);
            }
        }
    ]

    Connections {
        target: resource
        ignoreUnknownSignals: true
        onReviewSubmitFinished: {
            submitted = true;
        }
        onReviewSubmitFailed: {
            submitted = false;
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: Fluid.Units.smallSpacing

        Fluid.Label {
            text: qsTr("Find out what data in the <a href=\"#url\">privacy policy</a>")
            onLinkActivated: {
                Qt.openUrlExternally("https://odrs.gnome.org/privacy");
            }
        }

        ///

        Fluid.HeadlineLabel {
            text: qsTr("Rating")
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Repeater {
                model: 5

                Fluid.ToolButton {
                    property bool full: (hovering && hoverRating >= index + 1) || (!hovering && rating >= index + 1)

                    icon.source: Fluid.Utils.iconUrl("toggle/star" + (full ? "" : "_border"))
                    flat: true
                    hoverEnabled: true
                    enabled: !submitted

                    onHoveredChanged: {
                        if (hovered) {
                            hovering = true;
                            hoverRating = index + 1;
                        } else {
                            hovering = false;
                            hoverRating = 0;
                        }
                    }
                    onClicked: {
                        rating = index + 1;
                    }
                }
            }
        }

        ///

        Fluid.HeadlineLabel {
            text: qsTr("Summary")
        }

        ColumnLayout {
            Layout.fillWidth: true

            Fluid.TextField {
                Layout.fillWidth: true

                id: summaryField

                placeholderText: qsTr("Summary")
                focus: true
                enabled: !submitted
            }

            Fluid.Label {
                text: qsTr("Give a short summary of your review")
                color: Material.secondaryTextColor
            }
        }

        ///

        Fluid.HeadlineLabel {
            text: qsTr("Review")
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            Fluid.TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true

                id: descriptionField
                enabled: !submitted
            }

            Fluid.Label {
                text: qsTr("What do you think of the app?")
                color: Material.secondaryTextColor
            }
        }
    }
}
