import QtQuick 2.15
import QtQuick.Controls 2.12

Item {
    id: root
    Column {
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 30
        spacing: 10

        Item {
            anchors.left: parent.left
            anchors.right: parent.right
            height: 30
            TextField {
                id: docChooser
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.bottom: parent.bottom
                anchors.right: docChooserButton.left
            }
            Button {
                id: docChooserButton
                anchors.top: parent.top
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                width: 50
                text: "Go!"
                onPressed: {
                    links.model = theLinkProvider.getLinks(docChooser.text)
                    backLinks.model = theLinkProvider.getBackLinks(docChooser.text)
                }
            }
        }

        Text {
            id: linksLabel
            text: "Links"
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
        }
    }
}
