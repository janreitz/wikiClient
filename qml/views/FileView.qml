import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import "../components"

Item {
    id: root

    signal loadFile(string filePath)

    Item {
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 30

    }

    TreeView {
        id: treeView
        anchors.fill: parent
        TableViewColumn {
            title: "Name"
            role: "fileName"
            width: 300
        }
        model: theFileManager
        rootIndex: theFileManager.getCurrentPathIndex()
        itemDelegate: itemDelegate
        rowDelegate: rowDelegate
        headerDelegate: headerDelegate
    }

    Component {
        id: headerDelegate
        Rectangle {
            height: 50
            color: theme.colorAreaBackground
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.value
            color: theme.colorTextLight
            font: theme.fontSideBarHeader
        }
        }
    }

    Component {
        id: itemDelegate
        Text {
            anchors.fill: parent
            text: styleData.value
            color: styleData.selected ? theme.colorTextLightHighlight : theme.colorTextLight
            font: theme.fontSideBarNormal
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    var filePath = theFileManager.rootPath + "/" + parent.text;
                    console.log("FileView item clicked -> " + filePath)
                    root.loadFile(filePath)
                }
            }
        }
    }

    Component {
        id: rowDelegate
        Rectangle {
            color: styleData.selected ? theme.colorAreaHighlight : theme.colorAreaBackground
            height: 30//treeView.itemDelegate.height * 1.2
        }
    }

    Connections {
        target: theFileManager
        function onRootPathChanged() {
            console.log("rootPath changed")
            treeView.rootIndex = theFileManager.getCurrentPathIndex()
        }
    }
}
