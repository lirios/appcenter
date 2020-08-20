// SPDX-FileCopyrightText: 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

import QtQuick 2.0
import QtQuick.Controls 2.1

Dialog {
    id: addSourceDialog

    signal failed(string errorMessage)

    title: qsTr("Add Source")

    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    width: 500
    modal: true
    standardButtons: Dialog.Ok | Dialog.Cancel

    onVisibleChanged: {
        urlField.clear();
        urlField.forceActiveFocus();
    }

    onAccepted: {
        if (!softwareManager.addSource(urlField.text))
            addSourceDialog.failed(qsTr("Failed to retrieve source"));
    }

    TextField {
        id: urlField
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.right: parent.right
        placeholderText: qsTr("URL")
    }
}
