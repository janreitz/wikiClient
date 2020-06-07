import QtQuick 2.15
import QtGraphicalEffects 1.15

Item {
    id: root
    property string normalColor: "#ccc"
    property string hoveredColor: "#9c9"
    property alias source: icon.source
    property alias iconAnchorsMargins: icon.anchors.margins
    signal pressed()

    Image {
        id: icon
        anchors.fill: parent
        visible: false
    }

    MouseArea {
        id: mouseArea
        anchors.fill: root
        onClicked: root.pressed()
        hoverEnabled: true
//        onEntered: console.log("MouseArea entered")
    }

    ColorOverlay {
        source: icon
        anchors.fill: root
        color: mouseArea.containsMouse ? hoveredColor : normalColor
    }
}
