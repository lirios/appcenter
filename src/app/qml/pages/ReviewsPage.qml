// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter
import "../app" as App

FluidControls.Page {
    property alias resource: reviewsModel.resource

    title: qsTr("All reviews")

    BusyIndicator {
        anchors.centerIn: parent
        visible: reviewsModel.count == 0
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ListView {
            model: AppCenter.FilteredReviewsModel {
                id: reviewsModel
            }
            spacing: FluidControls.Units.smallSpacing
            delegate: App.Review {
                width: ListView.view.width
                height: Math.max(150, implicitHeight)
                drawBorder: false
            }
        }
    }
}
