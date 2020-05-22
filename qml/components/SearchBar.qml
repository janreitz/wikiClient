import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

Item {
    id: searchBar
    visible: true

    TextInput {
        id: searchBarInput
        anchors.fill: parent
        onTextChanged: searchContextMenu.open()
    }

    Menu {
        id: searchContextMenu

        MenuItem {
            text: "New..."
        }
        MenuItem {
            text: "Open..."
        }
        MenuItem {
            text: "Save"
        }
    }
}


