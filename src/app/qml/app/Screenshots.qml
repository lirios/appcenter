// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Fluid as Fluid
import Liri.AppCenter as AppCenter

Fluid.ScrollView {
    Layout.fillWidth: true
    Layout.preferredHeight: screenshotsModel.maximumThumbnailSize.height / 2

    clip: true

    ListView {
        orientation: ListView.Horizontal
        spacing: Fluid.Units.smallSpacing
        visible: count > 0
        model: AppCenter.ScreenshotsModel {
            id: screenshotsModel

            app: appPage.app.selectedResource
        }
        delegate: Image {
            id: thumbnail

            source: model.thumbnailUrl
            sourceSize.width: model.thumbnailSize.width / 2
            sourceSize.height: model.thumbnailSize.height / 2
            width: model.thumbnailSize.width / 2
            height: model.thumbnailSize.height / 2
            visible: status !== Image.Error

            Fluid.BusyIndicator {
                anchors.centerIn: parent
                visible: thumbnail.status == Image.Loading
            }

            Fluid.Ripple {
                anchors.fill: parent
                onClicked: screenshotOverlay.show(model.screenshotUrl)
            }
        }
    }
}
