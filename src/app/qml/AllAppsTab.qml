// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

FluidControls.Tab {
    title: qsTr("All Apps")

    ScrollView {
        anchors.fill: parent

        ListView {
            model: AppCenter.FilteredResourcesModel {
                sourceModel: softwareManager.resourcesModel
                filter: AppCenter.FilteredResourcesModel.AllApps
            }
            delegate: FluidControls.ListItem {
                text: model.resource.name
                subText: model.resource.summary
                onClicked: pageStack.push(Qt.resolvedUrl("AppPage.qml"), {app: model.resource})
            }
        }
    }
}
