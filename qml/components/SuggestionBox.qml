import QtQuick 2.14
import QtQuick.Controls 2.13
import QtQuick.Templates 2.14

FocusScope {
    id: container
    property alias focusedSuggestion: listView.currentItem
    property var suggestions: ["Suggestion1", "Suggestion2", "Suggestion3", "suggestion4"]
    signal signalSuggestionAccepted(string suggestion)
    property int suggestionHeight

    onFocusChanged: {
        //console.log("SuggestionBox::container -> " + (activeFocus ? "received " : "lost ") + "Focus");
        listView.focus = true;
    }

    Component {
        id: suggestionDelegate
        Rectangle {
            id: delegateRect
            anchors.left: parent.left
            anchors.right: parent.right
            height: container.suggestionHeight
            color: listView.activeFocus && ListView.isCurrentItem ? "gray" : "white"
            Text {
                anchors.fill: parent
                text: modelData
                leftPadding: 10
                verticalAlignment: Text.AlignVCenter
            }
            MouseArea {
                hoverEnabled: true
                anchors.fill: parent
                onClicked: {
                    if (mouse.button == Qt.LeftButton)
                    {
                        console.log("SuggestionBox::suggestionDelegate::onClicked -> Suggestion Accepted: " + modelData);
                        container.signalSuggestionAccepted(modelData)
                    }
                }
                onEntered: listView.currentIndex = index
            }

            Keys.onReturnPressed: {
                console.log("SuggestionBox::suggestionDelegate::onEnterPressed -> Suggestion Accepted: " + modelData);
                container.signalSuggestionAccepted(modelData)
                listView.currentIndex = 0
            }
        }
    }

    ListView {
        id: listView
        model: suggestions
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: childrenRect.height
        delegate: suggestionDelegate
        onCurrentIndexChanged: console.log("listView::onCurrentIndexChanged -> " + currentIndex)
        onFocusChanged: {
            console.log("SuggestionBox::listView -> " + (activeFocus ? "received " : "lost ") + "Focus");
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
