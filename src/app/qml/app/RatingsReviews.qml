// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Core 1.0 as FluidCore
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter
import "../ratings" as Ratings

ColumnLayout {
    Layout.fillWidth: true

    visible: app.rating || paginateModel.pageCount > 0

    RowLayout {
        Layout.fillWidth: true

        FluidControls.HeadlineLabel {
            Layout.fillWidth: true

            text: qsTr("Ratings and reviews")
        }

        ToolButton {
            ToolTip.text: qsTr("Show all reviews")
            ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

            icon.source: FluidControls.Utils.iconUrl("navigation/arrow_forward")
            flat: true
            hoverEnabled: FluidCore.Device.hoverEnabled

            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/ReviewsPage.qml"), {resource: app});
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter

        spacing: FluidControls.Units.smallSpacing * 2
        visible: app.rating

        Ratings.RatingNumber {
            Layout.alignment: Qt.AlignVCenter
        }

        Ratings.StarRatings {
            Layout.alignment: Qt.AlignVCenter
        }
    }

    BusyIndicator {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        visible: !reviewsModel.fetched
    }

    GridLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        rowSpacing: FluidControls.Units.smallSpacing
        columnSpacing: FluidControls.Units.smallSpacing
        flow: appPage.width > appPage.height ? GridLayout.LeftToRight : GridLayout.TopToBottom
        visible: reviewsModel.fetched && reviewsModel.count > 0

        Repeater {
            model: AppCenter.PaginateModel {
                id: paginateModel

                sourceModel: AppCenter.FilteredReviewsModel {
                    id: reviewsModel

                    property bool fetched: false

                    resource: app.selectedResource
                    onReviewsFetched: {
                        fetched = true;
                    }
                }
                pageSize: 2
            }

            Review {
                Layout.minimumWidth: (appPage.width / paginateModel.pageSize) - (FluidControls.Units.smallSpacing * 3)
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    RowLayout {
        Button {
            ToolTip.text: qsTr("Show all reviews")
            ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

            text: qsTr("More reviews...")
            flat: true
            highlighted: true
            hoverEnabled: FluidCore.Device.hoverEnabled
            visible: reviewsModel.fetched && reviewsModel.count > paginateModel.pageSize
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/ReviewsPage.qml"), {resource: app});
            }
        }

        Button {
            ToolTip.text: qsTr("Post a review of this resource")
            ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

            text: qsTr("Write a review...")
            flat: true
            highlighted: true
            hoverEnabled: FluidCore.Device.hoverEnabled
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/AddReviewPage.qml"), {resource: app});
            }
        }
    }
}
