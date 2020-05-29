import QtQuick 2.15

Rectangle {
    id: divider
    color: "yellow"

    property bool isVDivider: true

    DragHandler {
           id: dragHandler
           target: divider
           cursorShape: divider.isVDivider ? Qt.SplitVCursor : Qt.SplitHCursor
    }
}
