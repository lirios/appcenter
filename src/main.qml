import QtQuick 2.4
import QtQuick.Controls 1.2
import Material 0.1
import Material.ListItems 0.1 as ListItem
import Papyros.Software 0.1

ApplicationWindow {
    id: demo

    title: "App Store"

    // Necessary when loading the window from C++
    visible: true

    theme {
        primaryColor: "blue"
        accentColor: "red"
        tabHighlightColor: "white"
        backgroundColor: "white"
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

        Icon {
            opacity: 0.5
            name: "action/shop"
            size: Units.dp(96)
            anchors.centerIn: parent
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
                    source: edit.iconName.indexOf("/") == 0
                            ? edit.iconName : "image://desktoptheme/" + edit.iconName
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
}
