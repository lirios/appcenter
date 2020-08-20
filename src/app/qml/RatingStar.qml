// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0 as FluidControls

FluidControls.Icon {
    property int star: 1
    readonly property bool full: app.rating && Math.floor(app.rating.rating) >= star
    readonly property bool half: app.rating && star > Math.floor(app.rating.rating) && star <= Math.ceil(app.rating.rating)

    source: FluidControls.Utils.iconUrl("toggle/star" + (half ? "_half" : full ? "" : "_border"))
    size: 12

    color: full || half ? Material.accentColor : Material.secondaryTextColor
}
