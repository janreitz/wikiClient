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

    Rectangle {
        id: slideoutSidebar
        color: "blue"
        anchors.left: permanentSidebar.right
        anchors.top: permanentSidebar.top
        anchors.bottom: permanentSidebar.bottom
        width: 0
        property int slideoutWidth: 300
        state: "slideIn"
        states: [
            State {
                name: "slideIn"
                PropertyChanges {
                    target: slideoutSidebar
                    width: 0
                }
            },
            State {
                name: "slideOut"
                PropertyChanges {
                    target: slideoutSidebar
                    width: slideoutWidth
                }
            }
        ]

        transitions: [
            Transition {
                from: "slideIn"
                to: "slideOut"
                PropertyAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: 200  }
            },
            Transition {
                from: "slideOut"
                to: "slideIn"
                PropertyAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: 200  }
            }
        ]

        function toggleSlideout() {
            state = state == "slideIn" ? "slideOut" : "slideIn"
        }

        Connections {
            target: linksButton
            onPressed: {
                slideoutSidebar.toggleSlideout()
            }
        }
    }
}


