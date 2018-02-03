/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2018 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
 *
 * $BEGIN_LICENSE:GPL3+$
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * $END_LICENSE$
 ***************************************************************************/

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
