// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick
import QtQuick.Controls
import Fluid as Fluid
import Liri.AppCenter as AppCenter
import "app" as App

Fluid.Tab {
    title: qsTr("All Apps")

    AppCenter.FilteredResourcesModel {
        id: allAppsModel

        manager: softwareManager
        filter: AppCenter.FilteredResourcesModel.AllApps
    }

    Fluid.ScrollView {
        anchors.fill: parent
        clip: true

        Fluid.ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            header: Fluid.HeadlineLabel {
                text: qsTr("Available (%1)").arg(allAppsModel.count)
                padding: Fluid.Units.smallSpacing * 2
            }
            model: allAppsModel
            delegate: Fluid.ListItem {
                text: model.name
                subText: model.summary
                valueText: model.rating
                    ? qsTr("Rating %1").arg((model.rating ? model.rating.rating : 0).toFixed(1).toLocaleString(Qt.locale()))
                    : ""
                leftItem: App.Icon {
                    anchors.verticalCenter: parent.verticalCenter
                    iconName: model.iconName
                    iconUrl: model.iconUrl
                    iconSize: 48
                }
                rightItem: Fluid.Button {
                    anchors.verticalCenter: parent.verticalCenter
                    flat: true
                    text: qsTr("Get")
                    onClicked: {
                        if (!model.proxy.install()) {
                            console.error("Installation of", model.name, "has failed")
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
