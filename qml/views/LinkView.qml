import QtQuick 2.15
import QtQuick.Controls 2.12
import "../components"

Item {
    id: root

    Column {
        anchors.top: searchBar.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        spacing: 10

        Text {
            id: linksLabel
            text: "Links"
            font: theme.fontSideBarHeader
            color: theme.colorTextLight
        }

        ListView {
            id: links
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height
            model: ["link1", "link2", "link3"]
            delegate: linkDelegate
        }

        Text {
            id: backlinksLabel
            text: "Backlinks"
            font: theme.fontSideBarHeader
            color: theme.colorTextLight
        }

        ListView {
            id: backLinks
            anchors.left: parent.left
            anchors.right: parent.right
            height: childrenRect.height
            model: ["backlink1", "backlink2", "backlink3"]
            delegate: linkDelegate
        }
    }

    Component {
        id: linkDelegate
        Text {
            text: modelData
            font: theme.fontSideBarNormal
            color: linkMouseArea.containsMouse ? theme.colorTextLightHighlight : theme.colorTextLight
            MouseArea {
                id: linkMouseArea
                anchors.fill: parent
                hoverEnabled: true
            }
        }
    }

    Item {
        id: searchBar
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.topMargin: 20
        height: 30
        FocusScope {
            id: focusScope
            anchors.fill: parent

            TextField {
                id: searchInput
                focus: true
                objectName: "searchBarInput"
                anchors.left: parent.left
                anchors.leftMargin: 20
                anchors.right: searchButton.left
                anchors.rightMargin: 20
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                selectByMouse: true
                width: root.width * 0.7
                leftPadding: 10
                font: theme.fontSideBarNormal
                color: theme.colorTextLight
                verticalAlignment: Text.AlignVCenter
                background: Rectangle {
                    radius: 10
                    color: theme.colorAreaHighlight
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
                enabled: searchInput.activeFocus
                anchors.top: searchInput.bottom
                anchors.left: searchInput.left
                anchors.right: searchInput.right
                suggestionHeight: searchInput.height
                maxHeight: 400
                onSignalSuggestionAccepted: {
                    searchInput.text = suggestion;
                    searchInput.focus = true
                }
                onSignalExitedAbove: {
                    searchInput.focus = true
                }
            }

            Button {
                id: searchButton
                text: "Go"
                anchors.top: parent.top
                anchors.bottom: parent.bottom
                anchors.right: parent.right
                width: 50
                onPressed: {
                    links.model = theLinkProvider.getLinks(searchInput.text)
                    backLinks.model = theLinkProvider.getBackLinks(searchInput.text)
                }
            }
        }
    }

}
