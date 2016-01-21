import QtQuick 2.4
import QtQuick.Controls 1.2
import Material 0.2
import Material.ListItems 0.1 as ListItem
import Papyros.Software 0.1
import Papyros.Core 0.1

ApplicationWindow {
    id: demo

    title: "App Center"

    // Necessary when loading the window from C++
    visible: true

    theme {
        primaryColor: "blue"
        accentColor: "blue"
        tabHighlightColor: "white"
        // backgroundColor: "white"
    }

    function appIcon(name) {
        if (name == '')
            name = 'application-x-executable'

        if (name.indexOf('/') == 0)
            return name
        else
            return 'image://desktoptheme/' + name
    }

    Component.onCompleted: {
        if (session.shownWelcome == "false")
            welcomeDialog.open()
    }

    Action {
        id: searchAction

        iconName: "action/search"
        text: "Search"
    }

    initialPage: NavigationDrawerPage {
        id: navPage
        page: overviewPage

        navDrawer: NavigationDrawer {

            Column {
                anchors.fill: parent

                ListItem.Standard {
                    text: "Applications"
                    iconName: "action/shop"
                    selected: navPage.page == overviewPage
                    onClicked: navPage.navigate(overviewPage)
                }

                ListItem.Standard {
                    text: "Installed Apps"
                    iconName: "file/file_download"
                    selected: navPage.page == installedPage
                    onClicked: navPage.navigate(installedPage)
                }

                ListItem.Standard {
                    text: "Settings"
                    iconName: "action/settings"
                    selected: navPage.page == settingsPage
                    onClicked: navPage.navigate(settingsPage)
                }
            }
        }
    }

    Page {
        id: overviewPage
        title: "App Center"

        actions: [searchAction]

        Column {
            anchors.centerIn: parent
            spacing: Units.dp(16)
            opacity: 0.5

            Icon {
                name: "action/shop"
                size: Units.dp(96)
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                text: "Installing new applications is not supported yet"
                style: "title"
                anchors.horizontalCenter: parent.horizontalCenter
            }
        }
    }

    Page {
        id: installedPage
        title: "Installed Apps"

        actions: [searchAction]

        ListView {
            anchors.fill: parent

            model: software.installedApps
            delegate: ListItem.Subtitled {
                action: Image {
                    width: Units.dp(48)
                    height: width
                    anchors.centerIn: parent
                    source: appIcon(edit.iconName)
                    sourceSize {
                        width: Units.dp(48)
                        height: width
                    }
                }
                text: edit.name
                subText: edit.summary
                valueText: edit.branch
                onClicked: pageStack.push(Qt.resolvedUrl("ApplicationPage.qml"), {app: edit})
            }
        }
    }

    Page {
        id: settingsPage
        title: "Settings"
        actionBar.backgroundColor: Palette.colors.blueGrey['700']
        actionBar.decorationColor: Palette.colors.blueGrey['900']

        ListView {
            anchors.fill: parent

            model: software.remotes
            delegate: ListItem.Subtitled {
                text: edit.title ? "%1 (%2)".arg(edit.title).edit(edit.name) : edit.name
                subText: edit.url
            }
        }
    }

    Software {
        id: software
    }

    Dialog {
        id: welcomeDialog

        Column {
            spacing: Units.dp(16)

            Item {
                width: parent.width
                height: Units.dp(16)
            }

            Image {
                anchors.horizontalCenter: parent.horizontalCenter
                source: appIcon('software-store')
                sourceSize {
                    width: Units.dp(96)
                    height: width
                }
            }

            Label {
                style: "title"
                text: "Welcome to App Center"
                anchors.horizontalCenter: parent.horizontalCenter
            }

            Label {
                horizontalAlignment: Text.AlignJustify
                width: Units.dp(280)
                wrapMode: Text.Wrap
                style: "dialog"
                color: Theme.light.subTextColor
                text: "App Center lets you install all the software you need, all from one place. See our recommendations, browse the categories, or search for the applications you want."
            }
        }

        positiveButtonText: "Get Started"
        negativeButton.visible: false

        onAccepted: session.shownWelcome = "true"
    }

    KQuickConfig {
        id: session
        file: "papyros-appcenter"
        group: "session"

        defaults: {
            "shownWelcome": "false"
        }
    }
}
