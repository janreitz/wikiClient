import QtQuick 2.15
import QtQuick.Controls 2.14
import QtQuick.Layouts 1.3
import QtQuick.Dialogs 1.3

import Backend 1.0

FocusScope {
    id: root

    property var workArea
    property alias documentTitle: editorBackend.documentTitle
    property alias text: textArea.text
    property alias modified: editorBackend.modified
    property alias fileName: editorBackend.fileName
    function loadFileAbsolute(absolutePath) {
        console.log(objectName + "::scrollView::loadFile -> " + absolutePath)
        editorBackend.loadAbsolutePath(absolutePath)
    }
    function loadFileRelative(relativePath) {
        console.log(objectName + "::scrollView::loadFile -> " + relativePath)
        editorBackend.loadRelativePath(relativePath)
    }

//    onActiveFocusChanged: {
//        var focusReceivedOrLost = activeFocus ? "received" : "lost"
//        console.log(objectName + " active focus " + focusReceivedOrLost);
//    }

    ScrollView {
        id: scrollView
        clip: true
        anchors.fill: parent
        objectName: "Editor::scrollView"
        focus: true
//        onActiveFocusChanged: {
//            var focusReceivedOrLost = activeFocus ? "received" : "lost"
//            console.log(objectName + " active focus " + focusReceivedOrLost);
//        }


        TextArea {
            id: textArea
            objectName: scrollView.objectName + "::textArea"
            focus: true
            font: theme.fontTextBody
            color: theSettings.colorTextDark

            onActiveFocusChanged: {
//                var focusReceivedOrLost = activeFocus ? "received" : "lost"
//                console.log(objectName + " active focus " + focusReceivedOrLost);
                if (activeFocus) {root.workArea.lastActiveEditor = root}
            }
            background: Rectangle {
                color: textArea.activeFocus ? theSettings.colorAreaLightHighlight : theSettings.colorAreaLightBackground
            }
            textFormat: TextEdit.PlainText
            wrapMode: TextArea.Wrap
            tabStopDistance: 4
            selectByMouse: true
            persistentSelection: true
            placeholderText: "Your Awesome Wiki Article"

            Keys.onTabPressed: {
                editorBackend.tabPressed()
                event.accepted = true
            }

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
    }

    Rectangle {
        id: popupMessage
        anchors.horizontalCenter: root.horizontalCenter
        anchors.bottom: root.bottom
        anchors.bottomMargin: 20
        color: "#ccc"
        height: popupMessageText.implicitHeight + 5
        width: popupMessageText.implicitWidth + 20
        visible: false
        radius: height/4

        function show(message = "Popup Message", time = 3000){
            popupMessageText.text = message
            popupMessage.state = "on"
            popupTimer.interval = time
            popupTimer.start()
        }

        Text {
            id: popupMessageText
            anchors.centerIn: popupMessage
            font: theme.fontTextBody
            color: theSettings.colorTextDark
        }

        Timer {
            id: popupTimer
            onTriggered: popupMessage.state = "off"
        }

        states: [
            State {
                name: "on"
                PropertyChanges {
                    target: popupMessage
                    visible: true
                }
            },
            State {
                name: "off"
                PropertyChanges {
                    target: popupMessage
                    visible: false
                }
            }
        ]
    }


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
        onFileSaved: {
            popupMessage.show(fileName + " saved")
        }
    }

    MessageDialog {
        id: errorDialog
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
        sequence: "Ctrl+Shift+C"
        onActivated: editorBackend.inlineCode = !editorBackend.inlineCode
    }
    Shortcut {
        sequence: "Ctrl+Shift+M"
        onActivated: editorBackend.math = !editorBackend.math
    }
    Shortcut {
        sequence: "Ctrl+L"
        onActivated: editorBackend.addLinkTemplate()
    }
    Shortcut {
        sequence: "Ctrl+Alt+C"
        onActivated: editorBackend.addCodeBlock()
    }
    Menu {
        id: contextMenu
        objectName: "Editor::contextMenu"
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
