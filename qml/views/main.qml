import QtQuick 2.14
import QtQuick.Window 2.12
import "../components"
import QtQuick.Controls 2.13
import QtQuick.Dialogs.qml 1.0
import QtQuick.Layouts 1.3

Window {
    id: mainWindow
    visible: true
    width: 640
    height: 480
    title: qsTr("WikiClient")

    TabBar {
        id: tabBar
        height: 20
        contentWidth: 40
        contentHeight: 20
        anchors.top: searchbar.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TabButton {
               text: qsTr("Editor")
        }
        TabButton {
               text: qsTr("Blue")
        }
    }

    StackLayout {
        id: workArea
        anchors.top: tabBar.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right

        currentIndex: tabBar.currentIndex

        Editor {
            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Rectangle {
            id: blueTestRect
            Layout.fillHeight: true
            Layout.fillWidth: true
            color: "blue"
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    blueTestRect.color = blueTestRect.color == "#0000ff" ? "red" : "blue"
                }
            }
        }
    }

    SearchBar {
        id: searchbar
        height:30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
