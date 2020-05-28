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
            objectName: "tile_1"
        }
//        TestRect {
//            id: testRect_1;
//            focus: true
//            objectName: "tile_2"
//        }
    }

    Shortcut {
        sequence: "Alt+Shift+Down"
        onActivated: {
            console.log("TilingLayout::container -> Vertical split triggered")

            var focusItem = rootLayout.Window.activeFocusItem;

            var existingItem = firstParentWithObjectNameTile(focusItem)

            if (isChildOf(existingItem, rootLayout))
            {
                TilingManager.verticalSplit(
                    existingItem.parent,
                    existingItem,
                    "qrc:/qml/components/MultiTabEditor.qml")
            }
        }
    }
    Shortcut {
        sequence: "Alt+Shift+Right"
        onActivated: {
            console.log("TilingLayout::container -> Horizontal split triggered")
            var focusItem = rootLayout.Window.activeFocusItem;
            var existingItem = firstParentWithObjectNameTile(focusItem)

            if (isChildOf(existingItem, rootLayout))
            {
                TilingManager.horizontalSplit(
                    existingItem.parent,
                    existingItem,
                    "qrc:/qml/components/MultiTabEditor.qml")
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

    function firstParentWithObjectNameTile(child) {
        while (child)
        {
            if (child.objectName.match("tile")) { return child; }
            child = child.parent;
        }
        return false;
    }
}
