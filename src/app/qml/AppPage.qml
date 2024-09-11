// SPDX-FileCopyrightText: 2016 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as FluidC
import Liri.AppCenter as AppCenter
import "app" as App

Fluid.Page {
    id: appPage

    property AppCenter.ResourceProxy app: null

    title: app.name

    Connections {
        target: app
        ignoreUnknownSignals: true
        onReviewSubmitFinished: {
            snackBar.open(qsTr("Review successfully submitted"));
        }
        onReviewSubmitFailed: {
            snackBar.open(errorMessage);
        }
        onReviewReportFinished: {
            snackBar.open(qsTr("Review successfully submitted"));
        }
        onReviewReportFailed: {
            snackBar.open(errorMessage);
        }
        onReviewUpVoteFailed: {
            snackBar.open(errorMessage);
        }
        onReviewDownVoteFailed: {
            snackBar.open(errorMessage);
        }
        onReviewDismissFailed: {
            snackBar.open(errorMessage);
        }
        onReviewRemoveFinished: {
            snackBar.open(qsTr("Review successfully removed"));
        }
        onReviewRemoveFailed: {
            snackBar.open(errorMessage);
        }
    }

    Fluid.OverlayView {
        id: screenshotOverlay

        property alias source: screenshot.source

        anchors.centerIn: parent

        width: parent.width * 0.9
        height: parent.height * 0.9

        Image {
            id: screenshot

            anchors.fill: parent
            sourceSize.width: screenshot.sourceSize.width
            sourceSize.height: screenshot.sourceSize.height
            fillMode: Image.PreserveAspectFit

            MouseArea {
                anchors.fill: parent
                onClicked: screenshotOverlay.close()
            }
        }

        Fluid.BusyIndicator {
            anchors.centerIn: parent
            visible: screenshot.status != Image.Ready
        }

        function show(url) {
            source = url;
            open();
        }
    }

    Fluid.ScrollView {
        id: scrollView

        anchors.fill: parent
        contentWidth: pane.width
        contentHeight: pane.height
        clip: true

        Fluid.Pane {
            id: pane

            width: scrollView.width
            height: column.implicitHeight + topPadding + bottomPadding

            padding: Fluid.Units.smallSpacing * 2

            ColumnLayout {
                id: column

                spacing: Fluid.Units.smallSpacing
                width: parent.width

                App.Header {}

                App.Transaction {
                    id: transactionIndicator
                }

                App.Screenshots {}

                App.Info {}

                App.Details {}

                App.RatingsReviews {}
            }
        }
    }
}
