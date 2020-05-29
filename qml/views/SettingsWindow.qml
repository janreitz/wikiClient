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
        anchors.bottom: workingDirectorySettings.top
        anchors.bottomMargin: 10
        anchors.left: workingDirectorySettings.left
        anchors.leftMargin: 10
    }

    RowLayout {
        id: workingDirectorySettings
        anchors.verticalCenter : parent.verticalCenter
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.right: parent.right
        anchors.rightMargin: 10
        height: 50

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
