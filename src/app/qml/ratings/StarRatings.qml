// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import Fluid as Fluid

Grid {
    rows: 5
    columns: 2
    spacing: Fluid.Units.smallSpacing / 2
    horizontalItemAlignment: Grid.AlignRight
    verticalItemAlignment: Grid.AlignVCenter

    Row {
        Repeater {
            model: 5

            StarIcon {}
        }
    }

    Fluid.ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star5 : 0
    }

    ///

    Row {
        Repeater {
            model: 4

            StarIcon {}
        }
    }

    Fluid.ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star4 : 0
    }

    ///

    Row {
        Repeater {
            model: 3

            StarIcon {}
        }
    }

    Fluid.ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star3 : 0
    }

    ///

    Row {
        Repeater {
            model: 2

            StarIcon {}
        }
    }

    Fluid.ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star2 : 0
    }

    ///

    StarIcon {}

    Fluid.ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star1 : 0
    }
}
