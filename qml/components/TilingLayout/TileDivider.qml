import QtQuick 2.15

Rectangle {
    id: divider
    property string normalColor: "#ccc"
    property string activeColor: "#9c9"
    property int thickness: 10
    property bool isVDivider: true

    color: dragHandler.active ? activeColor : normalColor

    DragHandler {
           id: dragHandler
           target: divider
           cursorShape: divider.isVDivider ? Qt.SplitVCursor : Qt.SplitHCursor
    }

    states: [
        State {
            name: "vertical"
            when: isVDivider
            PropertyChanges {
                target: divider
                height: thickness
            }
        },
        State {
            name: "horizontal"
            when: !isVDivider
            PropertyChanges {
                target: divider
                width: thickness
            }
        }
    ]
}
