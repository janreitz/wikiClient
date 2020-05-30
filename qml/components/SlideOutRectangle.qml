import QtQuick 2.15

Rectangle {
    id: root
    color: "blue"
    width: 0
    property int slideoutWidth: 300
    property Item contentItem
    property Component contentComp

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
        contentItem.parent = this
    }

    onContentCompChanged: {
        var obj = contentComp.createObject(root)
        contentItem = obj
    }

    state: "slideIn"
    states: [
        State {
            name: "slideIn"
            PropertyChanges {
                target: root
                width: 0
            }
        },
        State {
            name: "slideOut"
            PropertyChanges {
                target: root
                width: slideoutWidth
            }
        }
    ]

    transitions: [
        Transition {
            from: "slideIn"
            to: "slideOut"
            PropertyAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: 200  }
        },
        Transition {
            from: "slideOut"
            to: "slideIn"
            PropertyAnimation { properties: "width"; easing.type: Easing.InOutQuad; duration: 200  }
        }
    ]
}
