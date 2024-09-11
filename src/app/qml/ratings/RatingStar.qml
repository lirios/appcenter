// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Fluid as Fluid

StarIcon {
    property int star: 1
    property real rating: 0
    readonly property bool full: {
        return Math.floor(rating) >= star;
    }
    readonly property bool half: {
        if (star > Math.floor(rating) && star <= Math.ceil(rating))
            return rating - Math.floor(rating) >= 0.5;
        return false;
    }

    source: Fluid.Utils.iconUrl("toggle/star" + (half ? "_half" : full ? "" : "_border"))
    color: full || half ? Material.accentColor : Material.secondaryTextColor
}
