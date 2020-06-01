import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Flickable {
    id: flickable
    flickableDirection: Flickable.VerticalFlick
    clip: true
    property alias text: textArea.text
    objectName: "Editor::flickable"

    TextArea.flickable: TextArea {
        id: textArea
        objectName: "Editor::textArea"
        Layout.fillHeight: true
        Layout.fillWidth: true
        leftPadding: 10
        rightPadding: 10
        topPadding: 10
        bottomPadding: 10
        focus: true
        onActiveFocusChanged: {
            var focusReceivedOrLost = activeFocus ? "received" : "lost"
            console.log(objectName + " active focus " + focusReceivedOrLost);
        }
        background: Rectangle {
            color: textArea.activeFocus ? "lightblue" : "pink"
        }
        textFormat: Qt.MarkdownText
        wrapMode: TextArea.Wrap
        selectByMouse: true
        persistentSelection: true
        placeholderText: "Your Awesome Wiki Article"

        MouseArea {
            id: mouseArea
            objectName: "Editor::mouseArea"
            height: parent.height
            width: parent.width
            acceptedButtons: Qt.RightButton
            onClicked: {
                if (mouse.button === Qt.RightButton)
                    contextMenu.popup()
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+M"
        onActivated: {
            textArea.textFormat = (textArea.textFormat == TextEdit.MarkdownText) ? TextEdit.NativeRendering : TextEdit.MarkdownText;
        }
    }

    Connections {
        target: editorFileDialog
        function onFileChosen(filePath) {
            textArea.text = theEditorBackend.readFile(Qt.resolvedUrl(filePath))
        }
    }

    Menu {
        id: contextMenu
        objectName: "Editor::contectMenu"
        MenuItem {
            text: "Open ..."
            onClicked: editorFileDialog.open()
        }
        MenuItem {
            text: "Toggle Rendering"
            onClicked: {
                textArea.textFormat = (textArea.textFormat == TextEdit.MarkdownText) ? TextEdit.NativeRendering : TextEdit.MarkdownText;
            }
        }
    }

    FileDialog {
        id: editorFileDialog
        objectName: "Editor::editorFileDialog"
        title: "Please choose a file"
        folder: shortcuts.home
        signal fileChosen(string filePath)
        onAccepted: this.fileChosen(this.fileUrl)
    }
    // SneakPreview
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
