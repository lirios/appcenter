// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.1
import QtQuick.Controls 2.0
import Fluid.Controls 1.0 as FluidControls

Grid {
    rows: 5
    columns: 2
    spacing: FluidControls.Units.smallSpacing / 2
    verticalItemAlignment: Grid.AlignVCenter

    Label {
        text: "5"
    }

    ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star5 : 0
    }

    Label {
        text: "4"
    }

    ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star4 : 0
    }

    Label {
        text: "3"
    }

    ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star3 : 0
    }

    Label {
        text: "2"
    }

    ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star2 : 0
    }

    Label {
        text: "1"
    }

    ProgressBar {
        from: 0
        to: app.rating ? app.rating.numVotes : 0
        value: app.rating ? app.rating.star1 : 0
    }
}
