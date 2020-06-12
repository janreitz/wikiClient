import QtQuick 2.15
import QtQuick.Controls 2.12
import Qt.labs.qmlmodels 1.0

Item {
    id: root


    TableView {
        id: linksContainer
        anchors.top: queryInput.bottom
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10

        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10

        clip: true

        model: theTableModelProvider.model

        delegate: Rectangle {
                implicitWidth: 100
                implicitHeight: 50
                border.width: 1

                Text {
                    text: display
                    anchors.centerIn: parent
                }
            }
    }

    Item {
        id: queryInput
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
                color: theSettings.colorTextLight
                verticalAlignment: Text.AlignVCenter
                background: Rectangle {
                    radius: 10
                    color: theSettings.colorAreaHighlight
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
                    theLinkProvider.documentTitle = searchInput.text;
                    //links.model = theLinkProvider.getLinks(searchInput.text)
                    //backLinks.model = theLinkProvider.getBackLinks(searchInput.text)
                }
            }
        }
    }

}
