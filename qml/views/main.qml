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

    TilingLayout {
        id: workArea
        anchors.top: searchbar.bottom
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        anchors.right: parent.right
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
