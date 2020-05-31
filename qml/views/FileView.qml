import QtQuick 2.15
import QtQuick.Controls 1.4

Item {
    id: root

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
    }

    Connections {
        target: theFileManager
        onRootPathChanged: {
            console.log("rootPath changed")
            treeView.rootIndex = theFileManager.getCurrentPathIndex()
        }
    }
}
