import QtQuick 2.4
import QtQuick.Window 2.2
import QtQuick.Layouts 1.0
import QtQuick.Controls 2.0
import QtQuick.Controls.Material 2.0
import Fluid.Controls 1.0
import Hawaii.Settings 1.0
import Papyros.Software 0.1

FluidWindow {
    id: demo

    title: qsTr("App Center")

    width: 800
    height: 600
    visible: true

    Material.primary: Material.Blue
    Material.accent: Material.Blue

    Component.onCompleted: {
        if (!settings.shownWelcome)
            welcomeDialog.open()
    }

    Action {
        id: searchAction

        iconName: "action/search"
        text: "Search"
    }

    initialPage: Page {
        title: qsTr("App Center")

        actions: [searchAction]

        Pane {
            id: listPane

            anchors {
                left: parent.left
                top: parent.top
                bottom: parent.bottom
            }

            Material.background: "white"
            Material.elevation: 1

            width: 200
            padding: 0
            z: 2

            ListView {
                anchors.fill: parent

                model: ListModel {
                    ListElement { text: qsTr("All Apps"); iconName: "action/shop"; url: "AllAppsPage.qml" }
                    ListElement { text: qsTr("Installed Apps"); iconName: "file/file_download"; url: "InstalledAppsPage.qml" }
                    ListElement { text: qsTr("Settings"); iconName: "action/settings"; url: "SettingsPage.qml" }
                }
                delegate: ListItem {
                    text: model.text
                    iconName: model.iconName
                    onClicked: pageStack.push(Qt.resolvedUrl(url))
                }

                ScrollBar.vertical: ScrollBar {}
            }
        }

        ListView {
            anchors {
                left: listPane.right
                top: parent.top
                right: parent.right
                bottom: parent.bottom
            }

            model: software.availableApps
            delegate: ListItem {
                text: edit.name
                subText: edit.summary
                valueText: edit.branch
                iconName: edit.icon
                onClicked: pageStack.push(Qt.resolvedUrl("ApplicationPage.qml"), {app: edit})
            }
        }
    }

    Popup {
        id: welcomeDialog
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2
        modal: true
        focus: true

        ColumnLayout {
            anchors.margins: 16
            spacing: 16

            Icon {
                name: "software-store"
                //name: "system-software-install"
                width: 96
                height: width

                Layout.alignment: Qt.AlignCenter
            }

            TitleLabel {
                text: qsTr("Welcome to App Center")

                Layout.alignment: Qt.AlignHCenter
            }

            BodyLabel {
                horizontalAlignment: Text.AlignJustify
                wrapMode: Text.WordWrap
                opacity: 0.5
                text: qsTr("App Center lets you install all the software you need, all from one place. " +
                           "See our recommendations, browse the categories, or search for the applications you want.")

                Layout.fillWidth: true
            }

            Button {
                text: qsTr("Get Started")
                // TODO: When QQC2 introduces a ButtonBox we'll likely have buttons automatically
                // flat for the Material style, in that case we can remove the line below
                flat: true
                onClicked: {
                    settings.shownWelcome = true
                    welcomeDialog.close()
                }

                Layout.alignment: Qt.AlignVCenter | Qt.AlignRight
            }
        }
    }

    SoftwareManager {
        id: software
    }

    Settings {
        id: settings
        schema.id: "io.papyros.AppCenter"
        schema.path: "/io/papyros/AppCenter/"
    }
}
