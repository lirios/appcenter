// SPDX-FileCopyrightText: 2020 Pier Luigi Fiorini <pierluigi.fiorini@gmail.com>
//
// SPDX-License-Identifier: GPL-3.0-or-later

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
            transaction = null;
        }

        function onCancelled() {
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
