// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

ScrollView {
    Layout.fillWidth: true
    Layout.preferredHeight: screenshotsModel.maximumThumbnailSize.height / 2

    clip: true

    ListView {
        orientation: ListView.Horizontal
        spacing: FluidControls.Units.smallSpacing
        visible: count > 0
        model: AppCenter.ScreenshotsModel {
            id: screenshotsModel

            app: appPage.app
        }
        delegate: Image {
            id: thumbnail

            source: model.thumbnailUrl
            sourceSize.width: model.thumbnailSize.width / 2
            sourceSize.height: model.thumbnailSize.height / 2
            width: model.thumbnailSize.width / 2
            height: model.thumbnailSize.height / 2
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
}
