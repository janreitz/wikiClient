import QtQuick 2.15
import QtQuick.Controls 2.12

Rectangle {
    id: root
    color: "gray"
    width: settingsButton.width * 1.2

    property alias settingsButton: settingsButton
    signal signalSettingsButtonPressed()

    Column {
        id: col
        spacing: 10
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.top: parent.top
        anchors.topMargin: 20
        RoundButton {
            text: "F"
        }
        RoundButton {
            text: "N"
        }
    }

    RoundButton {
        id: settingsButton
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 20
        icon.source: "qrc:/resources/icons/gear.svg"
        onPressed: root.signalSettingsButtonPressed()
    }
}
