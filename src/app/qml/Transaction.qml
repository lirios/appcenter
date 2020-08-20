/****************************************************************************
 * This file is part of App Center.
 *
 * Copyright (C) 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
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

import QtQuick 2.4
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.2
import Fluid.Controls 1.0 as FluidControls
import Liri.AppCenter 1.0 as AppCenter

RowLayout {
    id: transactionIndicator

    property AppCenter.Transaction transaction: null

    Layout.leftMargin: spacing
    Layout.rightMargin: spacing

    spacing: FluidControls.Units.smallSpacing * 2

    visible: transaction && transaction.visible

    // Hide immediately when the transaction is finished
    Connections {
        target: transaction
        ignoreUnknownSignals: true

        function onSucceeded() {
            transaction = null;
        }

        function onFailed() {
            snackBar.open(transaction.errorMessage);
        }

        function onCancelled() {
            transaction = null;
        }
    }

    // Hide after a while when the transaction has failed so
    // that the user will see the error
    Timer {
        running: transaction && transaction.status === AppCenter.Transaction.Failed
        interval: 5000
        onTriggered: {
            transaction = null;
        }
    }

    ColumnLayout {
        ProgressBar {
            from: 0.0
            to: 100.0
            value: transaction ? transaction.progress : 0.0
            indeterminate: {
                if (!transaction)
                    return false;

                switch (transaction.status) {
                case AppCenter.Transaction.Starting:
                case AppCenter.Transaction.Preparing:
                    return true;
                default:
                    return false;
                }
            }

            Layout.fillWidth: true
        }

        Label {
            text: {
                if (!transaction)
                    return "";

                switch (transaction.status) {
                case AppCenter.Transaction.Failed:
                    return transaction.errorMessage;
                default:
                    return transaction.description;
                }
            }
        }

        Layout.fillWidth: true
    }

    Button {
        text: qsTr("Cancel")
        enabled: {
            if (!transaction)
                return false;

            switch (transaction.status) {
            case AppCenter.Transaction.Failed:
            case AppCenter.Transaction.Canceled:
                return false;
            default:
                return true;
            }
        }
        visible: transaction && transaction.cancellable
        onClicked: {
            transaction.cancel();
        }
    }
}
