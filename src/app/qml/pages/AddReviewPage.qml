// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Page {
    property var resource
    property int rating: 0
    property int hoverRating: 0
    property bool hovering: false
    property bool submitted: false

    title: qsTr("Write a review for %1").arg(resource.name)

    padding: FluidControls.Units.smallSpacing * 2

    actions: [
        FluidControls.Action {
            icon.source: FluidControls.Utils.iconUrl("content/save")

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
        spacing: FluidControls.Units.smallSpacing

        Label {
            text: qsTr("Find out what data in the <a href=\"#url\">privacy policy</a>")
            onLinkActivated: {
                Qt.openUrlExternally("https://odrs.gnome.org/privacy");
            }
        }

        ///

        FluidControls.HeadlineLabel {
            text: qsTr("Rating")
        }

        RowLayout {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

            Repeater {
                model: 5

                ToolButton {
                    property bool full: (hovering && hoverRating >= index + 1) || (!hovering && rating >= index + 1)

                    icon.source: FluidControls.Utils.iconUrl("toggle/star" + (full ? "" : "_border"))
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

        FluidControls.HeadlineLabel {
            text: qsTr("Summary")
        }

        ColumnLayout {
            Layout.fillWidth: true

            TextField {
                Layout.fillWidth: true

                id: summaryField

                placeholderText: qsTr("Summary")
                focus: true
                enabled: !submitted
            }

            Label {
                text: qsTr("Give a short summary of your review")
                color: Material.secondaryTextColor
            }
        }

        ///

        FluidControls.HeadlineLabel {
            text: qsTr("Review")
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            TextArea {
                Layout.fillWidth: true
                Layout.fillHeight: true

                id: descriptionField
                enabled: !submitted
            }

            Label {
                text: qsTr("What do you think of the app?")
                color: Material.secondaryTextColor
            }
        }
    }
}
