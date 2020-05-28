import QtQuick 2.0
import QtQuick.Layouts 1.3

Rectangle {
    id: blueTestRect
    Layout.fillHeight: true
    Layout.fillWidth: true
    color: activeFocus ? "green" : "blue"
    onActiveFocusChanged: {
        if (activeFocus)
            console.log("TestRect::" + objectName + "::onActiveFocusChanged -> received active focus");
        else
            console.log("TestRect::" + objectName + "::onActiveFocusChanged -> lost active focus");
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            blueTestRect.color = blueTestRect.color == "#0000ff" ? "red" : "blue";
        }
    }
}
