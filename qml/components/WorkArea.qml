import QtQuick 2.15
import QtQuick.Controls 2.12
import "../js/TilingLayout.js" as TilingBackend
import "TilingLayout"
import QtQuick.Window 2.12

Item {
    id: root
    objectName: "TilingLayout::Root"
    Tile {
        id: rootTile
        objectName: "tile_root"
        color: "#2d2d2d"
        anchors.fill: parent
        contentItem: MultiTabEditor {
            parent: rootTile
            anchors.fill: parent
            anchors.margins: 5
        }
    }

    Component {
        id: editorTile
        MultiTabEditor {
            anchors.fill: parent
            anchors.margins: 5
        }
    }

    Shortcut {
        sequence: "Alt+Shift+Down"
        onActivated: {
            console.log("TilingLayout::container -> Vertical split triggered")
            var newContentItem = editorTile.createObject(root) // parent will be set in verticalSplit
            TilingBackend.verticalSplit(getActiveTile(), newContentItem)
        }
    }

    Shortcut {
        sequence: "Alt+Shift+Right"
        onActivated: {
            console.log("TilingLayout::container -> Horizontal split triggered")
            var newContentItem = editorTile.createObject(root) // parent will be set in horizontalSplit
            TilingBackend.horizontalSplit(getActiveTile(), newContentItem)
        }
    }

    function getActiveTile() {
        var focusItem = root.Window.activeFocusItem;
        var existingTile = focusItem;
        while (existingTile)
        {
            if (existingTile.hasOwnProperty('isTile')) {
                return existingTile
            };
            existingTile = existingTile.parent;
        }
        return false;
    }
}
