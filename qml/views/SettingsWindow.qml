import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12
import QtQuick.Layouts 1.12
import QtQuick.Dialogs 1.2

Window {
    visible: false
    width: 640
    height: 480

    Text {
        id: workingDirectoryLabel
        text: qsTr("Set working directory:")
    }

    RowLayout {
        id: workingDirectorySettings
        anchors.top: workingDirectoryLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right

        TextField {
            id: pathInput
            Layout.fillWidth: true
        }

        Button {
            id: setDirBrowseButton
            text: "Browse..."
            onPressed: fileDialog.open()
            Layout.preferredWidth: 100
        }

        Button {
            id: setDirButton
            text: "Set directory"
            Layout.preferredWidth: 100
            onPressed: {
                theFileManager.slotSetDirectory(pathInput.text)
                theFileManager.slotScanDirectory(pathInput.text)
            }
        }
    }

    FileDialog {
        id: fileDialog
        title: "Choose working directory"
        folder: shortcuts.home
        selectFolder: true
        onAccepted: pathInput.text = Qt.resolvedUrl(this.fileUrl)
    }
}
