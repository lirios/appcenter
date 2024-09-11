// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import Fluid as Fluid

Item {
    id: iconItem

    property real iconSize: 80
    property alias iconName: icon.name
    property alias iconUrl: image.source

    width: iconSize
    height: iconSize

    Fluid.Icon {
        id: icon

        size: iconItem.iconSize
        visible: !image.visible
    }

    Image {
        id: image

        width: iconItem.iconSize
        height: width
        sourceSize.width: width
        sourceSize.height: height
        visible: source.toString() !== ""
    }
}
