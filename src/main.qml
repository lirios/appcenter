import QtQuick 2.4
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
    }

    //Each will be an array populated in C++ that holds the apps in that category
    property var categories: [ games, utils, accessories, internet, videomusic, uniaccess, other ]

    property var categoryTitles: [ "Games", "Utilities", "Accessories", "Internet", "Music and Video", "Universal Access", "Other" ]

    initialPage: mainPage

    TabbedPage {
        id: mainPage

        title: "Software"

        actionBar.maxActionCount: navDrawer.enabled ? 3 : 4

        actions: [
            Action {
                iconName: "action/search"
                name: "Search"
                //onTriggered: TODO: Implement a Material searchbar
            },
            Action {
                iconName: "action/settings"
                name: "Settings"
                hoverAnimation: true
                onTriggered: {
                    pageStack.push(settings)
                 }
            }
        ]

        backAction: navDrawer.action

        NavigationDrawer {
            id: navDrawer

            enabled: mainPage.width < Units.dp(500)

            Flickable {
                anchors.fill: parent

                contentHeight: Math.max(content.implicitHeight, height)

                Column {
                    id: content
                    anchors.fill: parent

                    Repeater {
                        model: categories


                                delegate: ListItem.Standard {
                                    text: modelData
                                    selected: modelData == demo.selectedComponent
                                    onClicked: {
                                        demo.selectedComponent = modelData
                                        navDrawer.close()
                                    }
                                }
                            }
                }
            }
        }

        //Tabs
        Repeater {
            model: !navDrawer.enabled ? categoryTitles : 0

            delegate: Tab {
                title: modelData
                // TODO: filter by category // onclick:
            }
        }
    }

    Page {
        id: settings
        visible: false
        title: "App Store Settings"
    }

    XdgApp {
        Component.onCompleted: {
            listRemotes().forEach(function(remote) {
                console.log(remote.name +", " + remote.title +", " + remote.url)
            })
        }
    }
}
