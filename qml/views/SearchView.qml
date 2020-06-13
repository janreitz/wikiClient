import QtQuick 2.15
import QtQuick.Controls 2.12
import "../components"

Item {
    id:root

    FocusScope {
        id: focusScope
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 30

        TextField {
            id: searchBarInput
            focus: true
            objectName: "searchBarInput"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: searchTypeComboBox.left
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            selectByMouse: true
            width: root.width * 0.7
            leftPadding: 10
            font: theme.fontSideBarNormal
            color: theSettings.colorTextLight
            verticalAlignment: Text.AlignVCenter
            background: Rectangle {
                radius: 10
                color: theSettings.colorAreaHighlight
            }
            Keys.onPressed: {
                switch (event.key)
                {
                case Qt.Key_Down:
                    suggestionBox.focus = true
                    break;
                case Qt.Key_Return:
                    doSearch();
                    break;
                }
            }
            onTextChanged: {
                // suggestion according to search mode
                suggestionBox.suggestions = theTitleSuggestionProvider.getSuggestion(text)
            }
        }

        SuggestionBox {
            id: suggestionBox
            enabled: searchBarInput.activeFocus
            anchors.top: searchBarInput.bottom
            anchors.left: searchBarInput.left
            anchors.right: searchBarInput.right
            suggestionHeight: searchBarInput.height
            maxHeight: 400
            onSignalSuggestionAccepted: {
                queryInput.text = suggestion;
                searchBarInput.focus = true
            }
            onSignalExitedAbove: {
                searchBarInput.focus = true
            }
        }

        ComboBox {
            id: searchTypeComboBox
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: parent.right
            width: 100
            model: ["Full text", "Titles"]
        }
    }


    ListView {
        id: listView
        anchors.top: focusScope.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
        model: ["SearchResult", "SearchResult", "SearchResult"]
        delegate: searchResultDelegate
    }


    Component {
        id: searchResultDelegate
        ItemDelegate {
            id: delegateRoot
            width: ListView.view.width
            height: childrenRect.height
            property bool isSelected: false
            Text {
                id: titleDisplay
                text: modelData
                font: theme.fontSideBarNormal
                color: titleMouseArea.containsMouse ? theSettings.colorTextLightHighlight : theSettings.colorTextLight
                MouseArea {
                    id: titleMouseArea
                    anchors.fill: parent
                    hoverEnabled: true
                    onClicked: {
                        if (mouse.Button === Qt.LeftButton) {
                            delegateRoot.isSelected = true
                        }
                    }
                }
            }
            TextArea {
                id: matchContextDisplay
                anchors.top: titleDisplay.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: 0
                background: Rectangle {
                    anchors.fill: parent
                    color: theSettings.colorAreaHighlight
                }
                clip: true
                wrapMode: Text.WordWrap
                textFormat: "RichText"
                text: "Lorem ipsum <b>dolor sit amet</b>, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est Lorem ipsum dolor sit amet."
                font: theme.fontSideBarNormal
                color: theSettings.colorTextLight
            }

            states: [
                State {
                    name: "slideOut"
                    when: titleMouseArea.containsMouse || delegateRoot.isSelected
                    PropertyChanges {
                        target: matchContextDisplay
                        height: matchContextDisplay.implicitHeight
                    }
                }
            ]

            transitions: [
                Transition {
                    from: ""
                    to: "slideOut"
                    PropertyAnimation { properties: "height"; easing.type: Easing.InOutQuad; duration: 200  }
                },
                Transition {
                    from: "slideOut"
                    to: ""
                    PropertyAnimation { properties: "height"; easing.type: Easing.InOutQuad; duration: 200  }
                }
            ]
        }
    }

    function doSearch()
    {
        if (searchTypeComboBox.currentText == "Full text")
        {
            // Do full text search
            console.log("Full text search: ", searchBarInput.text);
        }
        else if (searchTypeComboBox.currentText == "SQL")
        {
            // Do SQL search
            console.log("SQL search: ", searchBarInput.text);
        }
        else if (searchTypeComboBox.currentText == "Titles")
        {
            // Do title search
            console.log("Titles search: ", searchBarInput.text);
        }
    }

}
