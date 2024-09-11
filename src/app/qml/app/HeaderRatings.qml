// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import "../ratings" as Ratings

ColumnLayout {
    visible: app.rating

    RowLayout {
        Fluid.DisplayLabel {
            level: 1
            text: (app.rating ? app.rating.rating.toFixed(1) : 0.0.toFixed(1)).toLocaleString(Qt.locale())
            color: Material.secondaryTextColor
        }

        RowLayout {
            Repeater {
                model: 5

                Ratings.RatingStar {
                    Layout.alignment: Qt.AlignVCenter

                    star: index + 1
                    rating: app.rating ? app.rating.rating : 0
                    size: 24
                    color: Material.secondaryTextColor
                }
            }
        }
    }

    Fluid.BodyLabel {
        level: 2
        text: qsTr("%1 Ratings").arg(Number(app.rating ? app.rating.numVotes : 0).toFixed(0).toLocaleString(Qt.locale()))
        color: Material.secondaryTextColor
    }
}
