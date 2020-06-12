import QtQuick 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3


FocusScope {
    id: root
    objectName: "MultiTabEditor::FocusScope"
    property var workArea

    onActiveFocusChanged: {
//        var focusReceivedOrLost = activeFocus ? "received" : "lost"
//        console.log(objectName + " active focus " + focusReceivedOrLost);
        if (activeFocus) {workArea.lastActiveMultiTabEditor = root}
    }

    TabBar {
        id: tabBar
        anchors.top: parent.top
        anchors.left: parent.left
        clip: true
        contentHeight: 30
        objectName: "MultiTabEditor::TabBar"
        TabButton {
            id: tabButton
            width: contentItem.childrenRect.width + 10
            property int index
            text: {
                var name = swipeView.itemAt(index).documentTitle;
                if (swipeView.itemAt(index).modified) {
                    name += " \u2B24"
                }
                return name;
            }
            contentItem: Item {
                Text {
                    id: tabTitle
                    text: tabButton.text
                    font: theme.fontTextBody
                    color: tabButton.checked ? theSettings.colorTextDark : theSettings.colorTextLight
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                IconButton {
                    id: closeButton
                    anchors.verticalCenter: tabTitle.verticalCenter
                    anchors.left: tabTitle.right
                    anchors.leftMargin: 10
                    height: tabTitle.height * 0.6
                    width: tabTitle.height * 0.6
                    normalColor: tabButton.checked ? theSettings.colorTextDark : theSettings.colorTextLight
                    hoveredColor: tabButton.checked ? theSettings.colorTextDarkHighlight : theSettings.colorTextLightHighlight
                    source: "qrc:/resources/icons/close.svg"
                    onPressed: {
                        closeTab(tabButton.index)
                    }
                }
            }
        }

        TabButton {
            id: newTabButton
            width: tabBar.contentHeight
            contentItem: IconButton {
                id: newTabIconButton
                anchors.fill:parent
                anchors.margins: tabBar.contentHeight * 0.3
                normalColor: newTabButton.checked ? theSettings.colorTextDark : theSettings.colorTextLight
                hoveredColor: newTabButton.checked ? theSettings.colorTextDarkHighlight : theSettings.colorTextLightHighlight
                source: "qrc:/resources/icons/plus.svg"
                onPressed: addNewTab()
            }
            onPressed: addNewTab()
        }
    }

    Component {
        id: tabButtonComp
        TabButton {
            id: tabButton
            width: contentItem.childrenRect.width + 10
            property int index
            text: {
                var name = swipeView.itemAt(index).documentTitle;
                if (swipeView.itemAt(index).modified) {
                    name += " \u2B24"
                }
                return name;
            }
            contentItem: Item {
                Text {
                    id: tabTitle
                    text: tabButton.text
                    font: theme.fontTextBody
                    color: tabButton.checked ? theSettings.colorTextDark : theSettings.colorTextLight
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }
                IconButton {
                    id: closeButton
                    anchors.verticalCenter: tabTitle.verticalCenter
                    anchors.left: tabTitle.right
                    anchors.leftMargin: 10
                    height: tabTitle.height * 0.6
                    width: tabTitle.height * 0.6
                    normalColor: tabButton.checked ? theSettings.colorTextDark : theSettings.colorTextLight
                    hoveredColor: tabButton.checked ? theSettings.colorTextDarkHighlight : theSettings.colorTextLightHighlight
                    source: "qrc:/resources/icons/close.svg"
                    onPressed: {
                        closeTab(tabButton.index)
                    }
                }
            }
        }
    }

    SwipeView {
        id: swipeView
        anchors.top: tabBar.bottom
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.bottom: parent.bottom
        focus: true
        clip: true
        objectName: "MultiTabEditor::SwipeView"
        currentIndex: tabBar.currentIndex
        interactive: false

        Editor {
            id: editor
            objectName: "MultiTabEditor::Editor_" + SwipeView.index
            workArea: root.workArea
        }
    }

    Component {
        id: editorComp
        Editor {
            id: editor
            objectName: "MultiTabEditor::Editor_" + SwipeView.index
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

    Shortcut {
        sequence: "Ctrl+T"
        onActivated: {
            addNewTab()
        }
    }

    Shortcut {
        sequence: "Ctrl+W"
        onActivated: closeTab(tabBar.currentIndex)
    }

    function addNewTab() {
//        console.log("adding New Tab")
        var insertIndex = tabBar.count - 1
        // new Editor
        swipeView.insertItem(insertIndex, editorComp.createObject(swipeView, {workArea: root.workArea}));
        // new Tab
        tabBar.insertItem(insertIndex, tabButtonComp.createObject(tabBar, {index: insertIndex}));
        tabBar.setCurrentIndex(insertIndex);
    }

    function openFileInNewTab(filePath) {
        addNewTab();
        swipeView.currentItem.loadPath(filePath);
    }

    function closeTab(index) {
        swipeView.removeItem(swipeView.itemAt(index));
        tabBar.removeItem(tabBar.itemAt(index));
    }
}


