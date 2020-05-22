import QtQuick 2.12
import QtQuick.Window 2.12
import "../components"

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("Hello World")

    SearchBar {
        id: searchbar
        height:30
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Editor {
        anchors.left: parent.left
        anchors.top: searchbar.bottom
    }
}
