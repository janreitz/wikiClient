import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

Item {
    id: textEditor
    visible: true
    width: 640
    height: 480

    ToolBar {
        id: toolBar
        anchors.left: textEditor.left
        anchors.top: textEditor.top
        anchors.right: textEditor.right
        RowLayout {
            ToolButton {
                text: "Bold"
            }
            ToolSeparator {

            }
            ToolButton {
                text: "Italic"
            }
            ToolSeparator {

            }
            ToolButton {
                text: "Add Link"
            }
        }
    }

    ScrollView {
        id: view
        anchors.top: toolBar.bottom
        anchors.left: textEditor.left
        anchors.right: textEditor.right
        anchors.bottom: textEditor.bottom

        TextArea {
            id: textArea
            placeholderText: qsTr("Text Area")
            anchors.fill: view
        }

        MouseArea {
            id: mouseTextArea
            anchors.fill:parent
            acceptedButtons: Qt.LeftButton | Qt.RightButton
                onClicked: {
                    if (mouse.button === Qt.RightButton)
                        contextMenu.popup()
                }
        }
        Menu {
            id: contextMenu
            MenuItem {text: "Copy"}
            MenuItem {text: "Cut"}
            MenuItem {text: "Paste"}
        }
    }
}

/*##^##
Designer {
    D{i:0;formeditorZoom:0.8999999761581421}
}
##^##*/
