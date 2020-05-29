import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3


Rectangle {
    color: "pink"
    anchors.fill:parent

    TabBar {
        id: editorTabBar
        anchors.top: parent.top
        anchors.left: parent.left
        contentHeight: 15
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

        anchors.top: editorTabBar.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom

        Editor {
            id: editor
            Layout.fillHeight:  true
            Layout.fillWidth: true

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
