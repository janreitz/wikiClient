import QtQuick 2.15

Item {
    id: root
    width: childrenRect.width
    property alias backgroundColor: contentArea.color
    property int slideoutWidth: 300
    property Item contentItem: defaultContent
    property Component contentComp
    state: "slideIn"

    // Removes the old contentItem from the renderScene
    function setContentItem(newContentItem) {
        if (contentItem) {
            contentItem.parent = null
            contentItem.width = 0
        }
        contentItem = newContentItem
    }

    function toggleSlideout() {
        state = state == "slideIn" ? "slideOut" : "slideIn"
    }

    function slideout() {
        if (state === "slideIn") {
            toggleSlideout()
        }
    }

    function slidein() {
        if (state === "slideOut") {
            toggleSlideout()
        }
    }

    onContentItemChanged: {
        contentItem.parent = contentArea
    }

    onContentCompChanged: {
        var obj = contentComp.createObject(contentArea)
        setContentItem(obj)
    }

    Rectangle {
        id: contentArea
        color: "blue"
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: dragHandle.left
    }

    Rectangle {
        id: dragHandle
        anchors.top: slideOutSidebar.top
        anchors.bottom: slideOutSidebar.bottom
        width: 10
        color: dragHandler.active ? "#9c9" : "#ccc"

        DragHandler {
            id: dragHandler
            target: dragHandle
            cursorShape: Qt.SplitHCursor
        }
    }

    states: [
        State {
            name: "slideIn"
            PropertyChanges {
                target: dragHandle
                x: 0
                width: 0
            }
            PropertyChanges {
                target: contentItem
                visible: false
            }
        },
        State {
            name: "slideOut"
            PropertyChanges {
                target: dragHandle
                x: slideoutWidth
                width: 10
            }
            PropertyChanges {
                target: contentItem
                visible: true
            }
        }
    ]

    transitions: [
        Transition {
            from: "slideIn"
            to: "slideOut"
            PropertyAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200  }
        },
        Transition {
            from: "slideOut"
            to: "slideIn"
            PropertyAnimation { properties: "x"; easing.type: Easing.InOutQuad; duration: 200  }
        }
    ]

    Rectangle {
        id: defaultContent
        color: "steelblue"
        objectName: "defaultContent"
        anchors.fill: parent
        anchors.margins: 20
    }
}
