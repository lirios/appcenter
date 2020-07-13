// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter
import "app" as App

FluidControls.Tab {
    title: qsTr("All Apps")

    AppCenter.FilteredResourcesModel {
        id: allAppsModel

        sourceModel: softwareManager.resourcesModel
        filter: AppCenter.FilteredResourcesModel.AllApps
    }

    ScrollView {
        anchors.fill: parent
        clip: true

        ScrollBar.horizontal.policy: ScrollBar.AlwaysOff

        ListView {
            header: FluidControls.HeadlineLabel {
                text: qsTr("Available (%1)").arg(allAppsModel.count)
                padding: FluidControls.Units.smallSpacing * 2
            }
            model: allAppsModel
            delegate: FluidControls.ListItem {
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
                rightItem: Button {
                    anchors.verticalCenter: parent.verticalCenter
                    flat: true
                    text: qsTr("Get")
                    onClicked: {
                        if (!model.resource.install()) {
                            console.error("Installation of", model.name, "has failed")
                            return;
                        }
                    }
                }
                onClicked: {
                    pageStack.push(Qt.resolvedUrl("AppPage.qml"), {app: model.resource});
                }
            }
        }
    }
}
