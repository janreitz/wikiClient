import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3


FocusScope {
    objectName: "MultiTabEditor::FocusScope"
    onActiveFocusChanged: {
        var focusReceivedOrLost = activeFocus ? "received" : "lost"
        console.log(objectName + " active focus " + focusReceivedOrLost);
    }
    TabBar {
        id: tabBar
        anchors.top: parent.top
        anchors.left: parent.left
        contentHeight: 30
        objectName: "MultiTabEditor::TabBar"

        TabButton {
            id: tabButton
            width: contentItem.implicitWidth + 10
            text: {
                var name = swipeView.currentItem.fileName==="" ? "untitled" : swipeView.currentItem.fileName;
                if (swipeView.currentItem.modified) {
                    name += " \u2B24"
                }
                return name;
            }
            contentItem: Text {
                text: tabButton.text
                font: theme.fontTextBody
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }
        TabButton {
            id: newTabButton
            text: "+"
            contentItem: Text {
                text: newTabButton.text
                font: theme.fontTextBody
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
            //onPressed: addNewTab()
        }
    }

    Component {
        id: tabButtonComp
        TabButton {
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        objectName: "MultiTabEditor::StackLayout"
        currentIndex: tabBar.currentIndex
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
            if (tabBar.currentIndex === tabBar.count - 1) {
                tabBar.setCurrentIndex(0);
            } else {
                tabBar.incrementCurrentIndex();
            }
        }
    }

//    function addNewTab() {

//    }
}


