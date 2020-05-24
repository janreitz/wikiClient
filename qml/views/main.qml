import QtQuick 2.12
import QtQuick.Window 2.12
import "../components"
import QtQuick.Controls 2.13
import QtQuick.Dialogs.qml 1.0
//import WikiClient.DataBackend 1.0

Window {
    visible: true
    width: 640
    height: 480
    title: qsTr("WikiClient")

    signal signalSetDirectory(string dirPath)
    signal signalScanDirectory()

    Shortcut {
        sequence: StandardKey.Open
        onActivated: signalSetDirectory(searchbar.text);
    }
    Shortcut {
        sequence: "Ctrl+R"
        onActivated: signalScanDirectory();
    }

    SearchBar {
        id: searchbar
        height:20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
    }

    Editor {
        anchors.left: parent.left
        anchors.top: searchbar.bottom
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }
}
