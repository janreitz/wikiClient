import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {

    Layout.fillHeight: true
    Layout.fillWidth: true

    Shortcut {
        sequence: "Ctrl+Tab"
        onActivated: {
            editorTabBar.incrementCurrentIndex();
        }
    }

    Connections {
        target: editorFileDialog
        onFileChosen: textArea.text = theEditorBackend.readFile(Qt.resolvedUrl(filePath))
    }

    TabBar {
        id: editorTabBar
        height: 20
        contentWidth: 40
        contentHeight: 20
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        Repeater {
            model: ["First", "Second", "Third", "Fourth", "Fifth"]
            TabButton {
                text: modelData
                width: implicitWidth
            }
        }
    }

    StackLayout {
        id: editorStackLayout
        currentIndex: 0

        anchors.fill: parent

//        anchors.top: editorTabBar.bottom
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom

        Editor {
        }

        MouseArea {
            id: mouseTextArea
            Layout.fillHeight: true
            Layout.fillWidth: true
            acceptedButtons: Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton)
                    contextMenu.popup()
            }
        }
    }

    Menu {
        id: contextMenu
        MenuItem {
            text: "Open ..."
            onClicked: editorFileDialog.open()
        }
    }

    FileDialog {
        id: editorFileDialog
        title: "Please choose a file"
        folder: shortcuts.home
        signal fileChosen(string filePath)
        onAccepted: this.fileChosen(this.fileUrl)
    }
}
