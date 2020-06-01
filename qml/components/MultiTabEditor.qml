import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3


FocusScope {
    objectName: "MultiTabEditor::FocusScope"
    onActiveFocusChanged: {
        var focusReceivedOrLost = activeFocus ? "received" : "lost"
        console.log(objectName + " active focus " + focusReceivedOrLost);
    }
    Rectangle {
        color: "pink"
        anchors.fill:parent

        TabBar {
            id: editorTabBar
            anchors.top: parent.top
            anchors.left: parent.left
            contentHeight: 15
            objectName: "MultiTabEditor::TabBar"
            Repeater {
                model: ["File1", "File2", "File3", "+"]
                TabButton {
                    text: modelData
                    height: 15
                }
            }
        }

        StackLayout {
            id: editorStackLayout
            focus: true
            anchors.top: editorTabBar.bottom
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.bottom: parent.bottom
            objectName: "MultiTabEditor::StackLayout"
            onActiveFocusChanged: {
                var focusReceivedOrLost = activeFocus ? "received" : "lost"
                console.log(objectName + " active focus " + focusReceivedOrLost);
            }

            Editor {
                id: editor
                focus: true
                Layout.fillHeight:  true
                Layout.fillWidth: true
                objectName: "MultiTabEditor::Editor"
            }
        }

        // Interactions

        Shortcut {
            sequence: "Ctrl+Tab"
            onActivated: {
                editorTabBar.incrementCurrentIndex();
            }
        }

    }
}


