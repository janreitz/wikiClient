import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    width: childrenRect.width

    Rectangle {
        id: permanentSidebar
        color: "gray"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom

        width: settingsButton.width * 1.2

        property alias settingsButton: settingsButton
        signal signalSettingsButtonPressed()

        Column {
            id: col
            spacing: 10
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.top: parent.top
            anchors.topMargin: 20
            RoundButton {
                id: filesButton
                text: "F"
            }
            RoundButton {
                id: linksButton
                text: "L"
            }
            RoundButton {
                id: networkButton
                text: "N"
            }
        }

        RoundButton {
            id: settingsButton
            anchors.horizontalCenter: parent.horizontalCenter
            anchors.bottom: parent.bottom
            anchors.bottomMargin: 20
            icon.source: "qrc:/resources/icons/gear.svg"
            onPressed: root.signalSettingsButtonPressed()
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


