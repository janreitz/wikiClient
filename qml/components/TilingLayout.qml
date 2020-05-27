import QtQuick 2.14
import QtQuick.Layouts 1.3
import QtQuick.Window 2.12
import "../js/TilingManager.js" as TilingManager

Item {
    id: container

    RowLayout {
        id: rootLayout
        objectName: "rootLayout"
        anchors.fill:parent;
        MultiTabEditor {
            objectName: "editor"
        }
        TestRect {
            id: testRect_1;
            focus: true
            objectName: "existingItem"
        }
    }

    Shortcut {
        sequence: "Alt++"
        onActivated: {
            console.log("TilingLayout::container -> Vertical split triggered")
            var focusItem = rootLayout.Window.activeFocusItem;
            console.log("TilingLayout::container -> activeFocusItem: " + focusItem.objectName)
            if (isChildOf(focusItem, rootLayout))
            {
                TilingManager.verticalSplit(
                            focusItem.parent,
                            focusItem,
                            "qrc:/qml/components/MultiTabEditor.qml",
                            {
                                text: "verticallySplit",
                            })
            }
        }
    }
    Shortcut {
        sequence: "Alt+-"
        onActivated: {
            console.log("TilingLayout::container -> Horizontal split triggered")
            var focusItem = rootLayout.Window.activeFocusItem;
            if (isChildOf(focusItem, rootLayout))
            {
                TilingManager.horizontalSplit(
                            focusItem.parent,
                            focusItem,
                            "qrc:/qml/components/MultiTabEditor.qml",
                            {
                                text: "horizontallySplit",
                            })
            }
        }
    }

    function isChildOf(child, parent) {
        while (child)
        {
            if (child === parent) { return true;}
            else {child = child.parent; }
        }
        return false;
    }
}
