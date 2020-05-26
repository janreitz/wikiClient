import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Item {
    id: textEditor

    Shortcut {
        sequence: "Ctrl+M"
        onActivated:
            textArea.textFormat = textArea.textFormat == TextEdit.MarkdownText ? TextEdit.AutoText : TextEdit.MarkdownText;
    }

    Shortcut {
        sequence: "Ctrl+Tab"
        onActivated: {
            editorTabBar.incrementCurrentIndex();
        }
    }

//    TabBar {
//        id: editorTabBar
//        height: 20
//        contentWidth: 40
//        contentHeight: 20
//        anchors.top: parent.top
//        anchors.left: parent.left
//        anchors.right: parent.right

//        Repeater {
//            model: ["First", "Second", "Third", "Fourth", "Fifth"]
//            TabButton {
//                text: modelData
//                width: implicitWidth
//            }
//        }
//    }

    StackLayout {
        id: edtorStackLayout
        currentIndex: 0

        anchors.fill: parent

//        anchors.top: editorTabBar.bottom
//        anchors.left: parent.left
//        anchors.right: parent.right
//        anchors.bottom: parent.bottom

        ScrollView {
            id: view
            Layout.fillHeight: true
            Layout.fillWidth: true

            TextArea {
                id: textArea
                placeholderText: qsTr("Text Area")
                anchors.fill: parent
                selectByMouse: true
                textFormat: TextEdit.MarkdownText

                Connections {
                    target: editorFileDialog
                    onFileChosen: textArea.text = theEditorBackend.readFile(Qt.resolvedUrl(filePath))
                }

                MouseArea {
                    id: mouseTextArea
                    anchors.fill:parent
                    acceptedButtons: Qt.RightButton
                    onClicked: {
                        if (mouse.button === Qt.RightButton)
                            contextMenu.popup()
                    }
                }
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

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
