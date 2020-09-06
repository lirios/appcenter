// SPDX-FileCopyrightText: 2016 Michael Spencer <sonrisesoftware@gmail.com>
// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter
import "app" as App

FluidControls.Page {
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

    FluidControls.OverlayView {
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

        BusyIndicator {
            anchors.centerIn: parent
            visible: screenshot.status != Image.Ready
        }

        function show(url) {
            source = url;
            open();
        }
    }

    ScrollView {
        id: scrollView

        anchors.fill: parent
        contentWidth: pane.width
        contentHeight: pane.height
        clip: true

        Pane {
            id: pane

            width: scrollView.width
            height: column.implicitHeight + topPadding + bottomPadding

            padding: FluidControls.Units.smallSpacing * 2

            ColumnLayout {
                id: column

                spacing: FluidControls.Units.smallSpacing
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
