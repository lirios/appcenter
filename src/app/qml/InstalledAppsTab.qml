// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import QtQuick.Controls.Material
import QtQuick.Layouts
import Fluid as Fluid
import Liri.Core as LiriCore
import Liri.AppCenter as AppCenter
import "app" as App

Fluid.Tab {
    title: qsTr("Installed Apps")

    AppCenter.FilteredResourcesModel {
        id: installedAppsModel

        manager: softwareManager
        filter: AppCenter.FilteredResourcesModel.InstalledApps
    }

    Fluid.ScrollView {
        anchors.fill: parent
        clip: true

        Fluid.ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            header: Fluid.HeadlineLabel {
                text: qsTr("On this device (%1)").arg(installedAppsModel.count)
                padding: Fluid.Units.smallSpacing * 2
            }
            model: installedAppsModel
            delegate: Fluid.ListItem {
                text: model.name
                subText: model.summary
                valueText: LiriCore.Formatter.formatByteSize(model.installedSize)
                leftItem: App.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    iconName: model.iconName
                    iconUrl: model.iconUrl
                    iconSize: 48
                }
                rightItem: Button {
                    anchors.verticalCenter: parent.verticalCenter
                    flat: true
                    text: qsTr("Open")
                    onClicked: {
                        if (!model.proxy.launch()) {
                            console.error("Execution of", model.name, "has failed")
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
