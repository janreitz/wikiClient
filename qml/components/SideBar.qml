import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    id: root
    width: childrenRect.width

    property alias settingsButton: settingsButton
    signal signalSettingsButtonPressed()

    Rectangle {
        id: permanentSidebar
        color: "#1E1E1E"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: 50

        Column {
            id: col
            spacing: 20
            anchors.horizontalCenter: permanentSidebar.horizontalCenter
            anchors.top: permanentSidebar.top
            anchors.topMargin: 20
            IconButton {
                id: filesButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/files_outline.svg"
                onPressed: root.signalSettingsButtonPressed()
            }
            IconButton {
                id: linksButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/link.svg"
                onPressed: root.signalSettingsButtonPressed()
            }
            IconButton {
                id: networkButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/network.svg"
                onPressed: root.signalSettingsButtonPressed()
            }
            IconButton {
                id: sqlButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/database.svg"
                onPressed: root.signalSettingsButtonPressed()
            }
        }

        IconButton {
            id: settingsButton
            anchors.horizontalCenter: permanentSidebar.horizontalCenter
            width: permanentSidebar.width * 0.7
            height: width
            anchors.bottom: permanentSidebar.bottom
            anchors.bottomMargin: 20
            source: "qrc:/resources/icons/gear_filled.svg"
            onPressed: permanentSidebar.signalSettingsButtonPressed()
        }
    }

    SlideOutRectangle {
        id: slideOutSidebar
        color: "pink"
        anchors.left: permanentSidebar.right
        anchors.top: permanentSidebar.top
        anchors.bottom: permanentSidebar.bottom
        slideoutWidth: 400
        contentItem: TestRect {
            anchors.fill: parent
            anchors.margins: 20
            color: "green"
        }
    }

//    StackView {
//        id: slideOutStackView
//        anchors.fill: parent
//    }

    Connections {
        target: linksButton
        function onPressed() {
            slideOutSidebar.toggleSlideout()
        }
    }
}


