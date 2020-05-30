import QtQuick 2.14
import QtQuick.Controls 2.13
import QtQuick.Templates 2.14

Rectangle {
    id: root
    color: "blue"
    property bool enabled
    property alias focusedSuggestion: listView.currentItem
    property var suggestions: ["Suggestion1", "Suggestion2", "Suggestion3", "suggestion4"]
    property int suggestionHeight
    property string highlightColor: "gray"
    property string normalColor: "white"
    signal signalSuggestionAccepted(string suggestion)
    signal signalExitedAbove()
    property int maxHeight: 9999

    onStateChanged: console.log("SuggestionBox state changed to " + state)

    onFocusChanged: {
        console.log("SuggestionBox::focusChanged")
        listView.forceActiveFocus()}

    Component {
        id: suggestionDelegate
        Rectangle {
            id: delegateRect
            property bool highlight: mouseArea.containsMouse || (listView.activeFocus && ListView.isCurrentItem)
            anchors.left: parent.left
            anchors.right: parent.right
            height: root.suggestionHeight
            color: highlight ? root.highlightColor : root.normalColor
            Text {
                anchors.fill: parent
                text: modelData
                leftPadding: 10
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                id: mouseArea
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if (mouse.button == Qt.LeftButton)
                    {
                        //console.log("SuggestionBox::suggestionDelegate::onClicked -> Suggestion Accepted: " + modelData);
                        root.signalSuggestionAccepted(modelData)
                    }
                }
                onEntered: listView.currentIndex = index
            }
            Keys.onReturnPressed: {
                //console.log("SuggestionBox::suggestionDelegate::onEnterPressed -> Suggestion Accepted: " + modelData);
                root.signalSuggestionAccepted(modelData)
                listView.currentIndex = 0
            }
        }
    }

    ListView {
        id: listView
        clip: true
        focus: true
        model: suggestions
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: childrenRect.height
        delegate: suggestionDelegate
        onCurrentIndexChanged: console.log("listView::onCurrentIndexChanged -> " + currentIndex)
        visible: root.state == "enabled" ? true : false
        onFocusChanged: {
            console.log("SuggestionBox::listView -> " + (activeFocus ? "received " : "lost ") + "Focus");
        }
        Keys.onUpPressed: {
            if (currentIndex == 0) {
                root.signalExitedAbove()
                event.accepted = true
            } else {
                event.accepted = false
            }
        }
    }
    states: [
        State {
            name: "enabled"
            when: enabled || listView.activeFocus
            PropertyChanges {
                target: root
                height: Math.min(root.maxHeight, listView.count * suggestionHeight)//listView.childrenRect.height)
            }
            PropertyChanges {
                target: listView
                height: Math.min(root.maxHeight, listView.count * suggestionHeight)//listView.childrenRect.height)
            }
        },
        State {
            name: ""
            PropertyChanges {
                target: root
                height: 0
            }
        }
    ]
    // Need to be revised, it appears ListViews cant smoothly drop down like this
//    transitions: [
//        Transition {
//            from: "enabled"
//            to: "disabled"
//            PropertyAnimation { properties: "height"; easing.type: Easing.InOutQuad; duration: 1000  }
//        },
//        Transition {
//            from: "disabled"
//            to: "enabled"
//            PropertyAnimation { properties: "height"; easing.type: Easing.InOutQuad; duration: 1000  }
//        }
//    ]
}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
