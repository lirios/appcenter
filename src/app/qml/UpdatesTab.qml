// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import Fluid as Fluid
import Liri.Core as LiriCore
import Liri.AppCenter as AppCenter
import "app" as App

Fluid.Tab {
    title: qsTr("Updates")

    AppCenter.FilteredResourcesModel {
        id: updatesModel

        manager: softwareManager
        filter: AppCenter.FilteredResourcesModel.Updates
    }

    Fluid.ScrollView {
        anchors.fill: parent
        clip: true

        Fluid.ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            header: Fluid.HeadlineLabel {
                text: qsTr("To update (%1)").arg(updatesModel.count)
                padding: Fluid.Units.smallSpacing * 2
            }
            model: updatesModel
            delegate: Fluid.ListItem {
                text: model.name
                subText: model.summary
                valueText: LiriCore.Formatter.formatByteSize(model.downloadSize)
                leftItem: App.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    iconName: model.iconName
                    iconUrl: model.iconUrl
                    iconSize: 48
                }
                rightItem: Fluid.Button {
                    anchors.verticalCenter: parent.verticalCenter
                    flat: true
                    text: qsTr("Update")
                    onClicked: {
                        if (!model.proxy.update()) {
                            console.error("Update of", model.name, "has failed")
                            return;
                        }
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AppPage.qml"), {app: model.proxy});
                }
            }
        }
    }
}
