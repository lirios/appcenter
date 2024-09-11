// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import Liri.AppCenter as AppCenter
import "../app" as App

Fluid.Page {
    property alias resource: reviewsModel.resource

    title: qsTr("All reviews")

    Fluid.BusyIndicator {
        anchors.centerIn: parent
        visible: reviewsModel.count == 0
    }

    Fluid.ScrollView {
        anchors.fill: parent
        clip: true

        ListView {
            model: AppCenter.FilteredReviewsModel {
                id: reviewsModel
            }
            spacing: Fluid.Units.smallSpacing
            delegate: App.Review {
                width: ListView.view.width
                height: Math.max(150, implicitHeight)
                drawBorder: false
            }
        }
    }
}
