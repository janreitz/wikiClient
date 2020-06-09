import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

import Backend 1.0

ScrollView {
    id: root
    clip: true
    property var workArea
    property alias documentTitle: editorBackend.documentTitle
    property alias text: textArea.text
    property alias modified: editorBackend.modified
    property alias fileName: editorBackend.fileName
    objectName: "Editor::flickable"
    function loadPath(filePath) {
        console.log("Editor::root::loadFile -> " + filePath)
        editorBackend.loadPath(filePath)
    }

//    onActiveFocusChanged: {
//        var focusReceivedOrLost = activeFocus ? "received" : "lost"
//        console.log(objectName + " active focus " + focusReceivedOrLost);
//    }

    EditorBackend {
        id: editorBackend
        document: textArea.textDocument
        cursorPosition: textArea.cursorPosition
        selectionStart: textArea.selectionStart
        selectionEnd: textArea.selectionEnd
        textColor: theSettings.colorTextDark
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

    TextArea {
        id: textArea
        objectName: root.objectName + "::textArea"
        focus: true
        font: theme.fontTextBody
        color: theSettings.colorTextDark

        onActiveFocusChanged: {
            var focusReceivedOrLost = activeFocus ? "received" : "lost"
            console.log(objectName + " active focus " + focusReceivedOrLost);
            if (activeFocus) {workArea.lastActiveEditor = root}
        }
        background: Rectangle {
            color: textArea.activeFocus ? theSettings.colorAreaLightHighlight : theSettings.colorAreaLightBackground
        }
        textFormat: TextEdit.PlainText
        wrapMode: TextArea.Wrap
        selectByMouse: true
        persistentSelection: true
        placeholderText: "Your Awesome Wiki Article"

        MouseArea {
            id: mouseArea
            objectName: "Editor::mouseArea"
            acceptedButtons: Qt.RightButton
            onClicked: {
                console.log(parent.objectName + "::mouseArea clicked")
                if (mouse.button === Qt.RightButton)
                    contextMenu.popup()
            }
        }
    }

    Shortcut {
        sequence: "Ctrl+M"
        onActivated: {
            textArea.textFormat = (textArea.textFormat == TextEdit.MarkdownText) ? TextEdit.PlainText : TextEdit.MarkdownText;
        }
    }
    Shortcut {
        sequence: StandardKey.Open
        onActivated: openDialog.open()
    }
    Shortcut {
        sequence: StandardKey.Save
        onActivated: onSave()
        onActivatedAmbiguously: {
            console.log("save triggered ambigously")
            if(textArea.activeFocus) { onSave() };
        }

        function onSave() {
            console.log("save triggered");

            if (editorBackend.currentFileUrlExists()) {
                editorBackend.saveAs(editorBackend.fileUrl);
            } else {
                saveDialog.open()
            }
        }
    }
    Shortcut {
        sequence: "Ctrl+Shift+S"
        onActivated: {
            console.log("saveas triggered");
            saveDialog.open()
        }
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
        MenuItem {
            text: "forceActiveFocus"
            onClicked: textArea.forceActiveFocus();
        }
    }


    FileDialog {
        id: openDialog
        onAccepted: editorBackend.loadUrl(fileUrl)
    }

    FileDialog {
        id: saveDialog
        defaultSuffix: editorBackend.fileType
        selectExisting: false
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
