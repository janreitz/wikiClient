import QtQuick 2.14
import QtQuick.Window 2.12
import "../components"
import QtQuick.Controls 2.13
import QtQuick.Dialogs.qml 1.0
import QtQuick.Layouts 1.3

Window {
    id: mainWindow
    visible: true
    width: 1280
    height: 960
    title: qsTr("WikiClient")

    Theme {
        id: theme
    }

    WorkArea {
        id: workArea
        anchors.top: searchBar.bottom
        anchors.left: sideBar.right
        anchors.bottom: parent.bottom
        anchors.right: parent.right
    }

    SideBar {
        id: sideBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.bottom: parent.bottom
    }

    SearchBar {
        id: searchBar
        height:30
        anchors.top: parent.top
        anchors.left: sideBar.right
        anchors.right: parent.right
    }

    SettingsWindow {
        id: settingsWindow
        visible: false
    }

    Connections {
        target: sideBar
        function onSignalSettingsButtonPressed() {
            console.log("Button pressed")
            settingsWindow.visible = true
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.6600000262260437}
}
##^##*/
