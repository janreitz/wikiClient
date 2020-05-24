import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3

import WikiClient.Suggestions 1.0

Item {
    id: searchBar
    visible: true
    signal signalSearch(string searchString)

    property alias text: searchBarInput.text

    TextInput {
        id: searchBarInput
        anchors.fill: parent
        //onTextChanged: searchContextMenu.open()
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

    Button {
        id: searchButton
        text: "Search"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        onClicked: searchBar.signalSearch(searchBarInput.text)
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
