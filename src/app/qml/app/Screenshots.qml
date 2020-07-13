// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

ListView {
    ScrollBar.horizontal: ScrollBar {}
    Layout.fillWidth: true
    Layout.preferredHeight: 120

    clip: true
    orientation: ListView.Horizontal
    spacing: FluidControls.Units.smallSpacing
    visible: count > 0
    model: AppCenter.ScreenshotsModel {
        app: appPage.app
    }
    delegate: Image {
        id: thumbnail

        source: model.thumbnailUrl

        width: height * (model.thumbnailSize.width / model.thumbnailSize.height)
        height: ListView.view.height
        visible: status !== Image.Error

        BusyIndicator {
            anchors.centerIn: parent
            visible: thumbnail.status == Image.Loading
        }

        FluidControls.Ripple {
            anchors.fill: parent
            onClicked: screenshotOverlay.show(model.screenshotUrl)
        }
    }
}
