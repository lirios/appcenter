// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.3
import QtQuick.Controls 2.2
import QtQuick.Controls.Material 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.Core 1.0 as LiriCore
import Liri.AppCenter 1.0 as AppCenter
import "app" as App

FluidControls.Tab {
    title: qsTr("Installed Apps")

    AppCenter.FilteredResourcesModel {
        id: installedAppsModel

        manager: softwareManager
        filter: AppCenter.FilteredResourcesModel.InstalledApps
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            header: FluidControls.HeadlineLabel {
                text: qsTr("On this device (%1)").arg(installedAppsModel.count)
                padding: FluidControls.Units.smallSpacing * 2
            }
            model: installedAppsModel
            delegate: FluidControls.ListItem {
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
