import QtQuick 2.15
import QtQuick.Controls 1.4

Item {
    id: root

    TreeView {
        anchors.fill: parent
        TableViewColumn {
            title: "Name"
            role: "fileName"
            width: 300
        }
        model: theFileManager
        rootIndex: theFileManager.getCurrentPathIndex()
    }
}
