// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid.Core as FluidCore
import Fluid as Fluid
import Liri.AppCenter as AppCenter
import "../ratings" as Ratings

ColumnLayout {
    Layout.fillWidth: true

    visible: app.rating || paginateModel.pageCount > 0

    RowLayout {
        Layout.fillWidth: true

        Fluid.HeadlineLabel {
            Layout.fillWidth: true

            text: qsTr("Ratings and reviews")
        }

        Fluid.ToolButton {
            Fluid.ToolTip.text: qsTr("Show all reviews")
            Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

            icon.source: Fluid.Utils.iconUrl("navigation/arrow_forward")
            flat: true
            hoverEnabled: FluidCore.Device.hoverEnabled

            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/ReviewsPage.qml"), {resource: app});
            }
        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignHCenter

        spacing: Fluid.Units.smallSpacing * 2
        visible: app.rating

        Ratings.RatingNumber {
            Layout.alignment: Qt.AlignVCenter
        }

        Ratings.StarRatings {
            Layout.alignment: Qt.AlignVCenter
        }
    }

    Fluid.BusyIndicator {
        Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

        visible: !reviewsModel.fetched
    }

    GridLayout {
        Layout.fillWidth: true
        Layout.fillHeight: true

        rowSpacing: Fluid.Units.smallSpacing
        columnSpacing: Fluid.Units.smallSpacing
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
                Layout.minimumWidth: (appPage.width / paginateModel.pageSize) - (Fluid.Units.smallSpacing * 3)
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    RowLayout {
        Fluid.Button {
            Fluid.ToolTip.text: qsTr("Show all reviews")
            Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

            text: qsTr("More reviews...")
            flat: true
            highlighted: true
            hoverEnabled: FluidCore.Device.hoverEnabled
            visible: reviewsModel.fetched && reviewsModel.count > paginateModel.pageSize
            onClicked: {
                pageStack.push(Qt.resolvedUrl("../pages/ReviewsPage.qml"), {resource: app});
            }
        }

        Fluid.Button {
            Fluid.ToolTip.text: qsTr("Post a review of this resource")
            Fluid.ToolTip.delay: FluidCore.Device.isMobile ? Qt.styleHints.mousePressAndHoldInterval : 0
            Fluid.ToolTip.visible: FluidCore.Device.isMobile ? pressed : hovered

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
