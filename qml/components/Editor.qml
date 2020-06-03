import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

import Backend 1.0

Flickable {
    id: flickable
    flickableDirection: Flickable.VerticalFlick
    clip: true
    property alias text: textArea.text
    objectName: "Editor::flickable"

    EditorBackend {
        id: editorBackend
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: theme.colorTextDark
        Component.onCompleted: editorBackend.load("C:/Users/jan-r/projects/wikiClient/demo/a-conflict-free-replicated-json-data-type.md")
        onLoaded: {
            textArea.text = text
        }
        onError: {
            errorDialog.text = message
            errorDialog.visible = true
        }
    }

    MessageDialog {
        id: errorDialog
    }

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
        font: theme.fontTextBody
        color: theme.colorTextDark

        onActiveFocusChanged: {
            var focusReceivedOrLost = activeFocus ? "received" : "lost"
            console.log(objectName + " active focus " + focusReceivedOrLost);
        }
        background: Rectangle {
            color: textArea.activeFocus ? "lightblue" : "pink"
        }
        textFormat: TextEdit.MarkdownText
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
    Shortcut {
        sequence: StandardKey.Open
        onActivated: openDialog.open()
    }
    Shortcut {
        sequence: StandardKey.SaveAs
        onActivated: saveDialog.open()
    }
    Shortcut {
        sequence: StandardKey.Quit
        onActivated: close()
    }
    Shortcut {
        sequence: StandardKey.Copy
        onActivated: textArea.copy()
    }
    Shortcut {
        sequence: StandardKey.Cut
        onActivated: textArea.cut()
    }
    Shortcut {
        sequence: StandardKey.Paste
        onActivated: textArea.paste()
    }
    Shortcut {
        sequence: StandardKey.Bold
        onActivated: editorBackend.bold = !editorBackend.bold
    }
    Shortcut {
        sequence: StandardKey.Italic
        onActivated: editorBackend.italic = !editorBackend.italic
    }
    Shortcut {
        sequence: StandardKey.Underline
        onActivated: editorBackend.underline = !editorBackend.underline
    }

    Menu {
        id: contextMenu
        objectName: "Editor::contectMenu"
        MenuItem {
            text: "Open ..."
            onClicked: openDialog.open()
        }
        MenuItem {
            text: "Toggle Rendering"
            onClicked: {
                textArea.textFormat = (textArea.textFormat == TextEdit.MarkdownText) ? TextEdit.NativeRendering : TextEdit.MarkdownText;
            }
        }
    }


    FileDialog {
        id: openDialog
        onAccepted: editorBackend.load(fileUrl)
    }

    FileDialog {
        id: saveDialog
        defaultSuffix: editorBackend.fileType
        nameFilters: openDialog.nameFilters
        onAccepted: editorBackend.saveAs(fileUrl)
    }
    // SneakPreview
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
