import QtQuick 2.15
import QtQuick.Controls 1.4
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4
import QtQuick.Dialogs 1.2

import "../components"

Item {
    id: root

    property var lastActiveEditor

    Item {
        id: buttonContainer
        anchors.top: parent.top
        anchors.topMargin: 30
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 30

        Button {
            text: "Choose Directory"
            onPressed: chooseDirDialog.open()
        }
    }

    FileDialog {
        id: chooseDirDialog
        selectFolder: true
        onAccepted: {
            theFileManager.workingDirectory = fileUrl;
        }
        folder: shortcuts.home
    }

    TreeView {
        id: treeView
        anchors.top: buttonContainer.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: parent.bottom
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
            color: theSettings.colorAreaBackground
        Text {
            anchors.verticalCenter: parent.verticalCenter
            text: styleData.value
            color: theSettings.colorTextLight
            font: theme.fontSideBarHeader
        }
        }
    }

    Component {
        id: itemDelegate
        Text {
            anchors.fill: parent
            text: styleData.value
            color: styleData.selected ? theSettings.colorTextLightHighlight : theSettings.colorTextLight
            font: theme.fontSideBarNormal
            verticalAlignment: Text.AlignVCenter
            MouseArea {
                anchors.fill: parent
                onClicked: {
                    root.lastActiveEditor.loadFileRelative(parent.text)
                }
            }
        }
    }

    Component {
        id: rowDelegate
        Rectangle {
            color: styleData.selected ? theSettings.colorAreaHighlight : theSettings.colorAreaBackground
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
