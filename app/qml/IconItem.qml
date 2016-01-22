import QtQuick 2.4
import QtQuick.Window 2.2
import org.kde.kquickcontrolsaddons 2.0

Item {
    property alias icon: __icon.icon

    QIconItem {
        id: __icon

        property real ratio: Screen.devicePixelRatio

        scale: 1/ratio
        width: parent.width * ratio
        height: parent.height * ratio

        anchors.centerIn: parent
    }
}
