import QtQuick 2.0

FocusScope {
    property Item contentItem
    property bool isTile: true
    property bool isSplitVertically: false
    property bool isSplitHorizontally: false
    property Item sibling
    // First ist either Top or Left
    property Item firstChild
    // Second ist either Bottom or Right
    property Item secondChild
    property Item divider
    property bool isTop: false
    property bool isBottom: false
    property bool isLeft: false
    property bool isRight: false

    property alias color: background.color

//    onActiveFocusChanged: {
//        var focusReceivedOrLost = activeFocus ? "received" : "lost"
//        console.log(objectName + " activeFocus " + focusReceivedOrLost);
//    }

    Rectangle {
        id: background
        anchors.fill: parent
        objectName: "tileBackground"
    }
}
