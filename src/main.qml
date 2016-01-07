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

    initialPage: TabbedPage {
        title: "Software"

        Tab {
            title: "Sources"

            ListView {
                anchors.fill: parent

                model: software.remotes
                delegate: ListItem.Subtitled {
                    text: edit.title ? "%1 (%2)".arg(edit.title).edit(edit.name) : edit.name
                    subText: edit.url
                }
            }
        }
    }

    Software {
        id: software
    }
}
