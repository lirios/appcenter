// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import Fluid as Fluid

ColumnLayout {
    Layout.fillWidth: true

    spacing: Fluid.Units.smallSpacing
    visible: descrLabel.visible && linksRow.visible

    Fluid.BodyLabel {
        Layout.fillWidth: true

        id: descrLabel

        text: app.description
        level: 2
        wrapMode: Label.WordWrap
        visible: text !== ""
    }

    RowLayout {
        id: linksRow

        spacing: Fluid.Units.smallSpacing * 2
        visible: visibleChildren.length > 0

        Fluid.BodyLabel {
            text: qsTr("<a href=\"%1\">Website</a>").arg(app.homepageUrl)
            visible: app.homepageUrl.toString() !== ""
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Fluid.BodyLabel {
            text: qsTr("<a href=\"%1\">Support</a>").arg(app.bugtrackerUrl)
            visible: app.bugtrackerUrl.toString() !== ""
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Fluid.BodyLabel {
            text: qsTr("<a href=\"%1\">Help</a>").arg(app.helpUrl)
            visible: app.helpUrl.toString() !== ""
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Fluid.BodyLabel {
            text: qsTr("<a href=\"%1\">FAQ</a>").arg(app.faqUrl)
            visible: app.faqUrl.toString() !== ""
            onLinkActivated: Qt.openUrlExternally(link)
        }

        Fluid.BodyLabel {
            text: qsTr("<a href=\"%1\">Donate</a>").arg(app.donationUrl)
            visible: app.donationUrl.toString() !== ""
            onLinkActivated: Qt.openUrlExternally(link)
        }
    }

    Fluid.ThinDivider {
        Layout.fillWidth: true
    }
}
