import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

Flickable {
    id: flickable
    flickableDirection: Flickable.VerticalFlick
    property alias text: textArea.text

    TextArea.flickable: TextArea {
        id: textArea
        Layout.fillHeight: true
        Layout.fillWidth: true
        leftPadding: 10
        rightPadding: 10
        topPadding: 10
        bottomPadding: 10
        background: Rectangle {
            color: "lightblue"
        }
        textFormat: Qt.MarkdownText
        wrapMode: TextArea.Wrap
        selectByMouse: true
        persistentSelection: true
        placeholderText: "Your Awesome Wiki Article"
        activeFocusOnTab: true
        onActiveFocusChanged: {
            if (activeFocus)
                console.log("Editor::" + objectName + "::onActiveFocusChanged -> received active focus");
            else
                console.log("Editor::" + objectName + "::onActiveFocusChanged -> lost active focus");
        }
        MouseArea {
            id: mouseArea
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
        function onFileChosen() {
            textArea.text = theEditorBackend.readFile(Qt.resolvedUrl(filePath))
        }
    }

    Menu {
        id: contextMenu
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
