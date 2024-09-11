// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQml
import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Fluid as Fluid

Column {
    spacing: Fluid.Units.smallSpacing / 2

    Fluid.DisplayLabel {
        level: 3
        text: (app.rating ? app.rating.rating.toFixed(1) : 0.0.toFixed(1)).toLocaleString(Qt.locale())
    }

    Row {
        Repeater {
            model: 5

            RatingStar {
                star: index + 1
                rating: app.rating ? app.rating.rating : 0
            }
        }
    }

    Fluid.BodyLabel {
        text: qsTr("%1 Ratings").arg(Number(app.rating ? app.rating.numVotes : 0).toFixed(0).toLocaleString(Qt.locale()))
    }
}
