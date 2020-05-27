import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Item {
    id: searchBar
    visible: true
    signal signalSearch(string searchString)

    Shortcut {
        sequence: StandardKey.Open
        onActivated: {
            theFileManager.slotSetDirectory(searchBarInput.text);
        }
    }
    Shortcut {
        sequence: "Ctrl+R"
        onActivated: theFileManager.slotScanDirectory();
    }

    TextField {
        id: searchBarInput
        objectName: "searchBarInput"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        selectByMouse: true
        width: parent.width * 0.7
        leftPadding: 10
        font.pointSize: 12
        verticalAlignment: Text.AlignVCenter
        Keys.onPressed: {
            switch (event.key)
            {
            case Qt.Key_Down:
                //console.log("Searchbar DownArrow");
                suggestionBox.focus = true;
                break;
            case Qt.Key_Return
            :
                doSearch();
                break;
            }
        }
        onTextChanged: {
            // suggestion according to search mode
            suggestionBox.suggestions = theTitleSuggestionProvider.getSuggestion(text)
        }
    }

    SuggestionBox {
        id: suggestionBox
        visible: searchBarInput.activeFocus || activeFocus //suggestions.length > 0
        height: visible ? childrenRect.height : 0
        anchors.top: searchBarInput.bottom
        anchors.left: searchBarInput.left
        anchors.right: searchBarInput.right
        suggestionHeight: searchBarInput.height
        onSignalSuggestionAccepted: {
            searchBarInput.text = suggestion;
            searchBarInput.focus = true;
        }
    }

    ComboBox {
        id: searchTypeComboBox
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: searchBarInput.right
        model: ["Full text", "Titles", "SQL"]
    }

    Button {
        id: searchButton
        text: "Go"
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        anchors.left: searchTypeComboBox.right
        onClicked: doSearch()
        Keys.onReturnPressed: doSearch()
    }

    function doSearch()
    {
        if (searchTypeComboBox.currentText == "Full text")
        {
            // Do full text search
            console.log("Full text search: ", searchBarInput.text);
        }
        else if (searchTypeComboBox.currentText == "SQL")
        {
            // Do SQL search
            console.log("SQL search: ", searchBarInput.text);
        }
        else if (searchTypeComboBox.currentText == "Titles")
        {
            // Do title search
            console.log("Titles search: ", searchBarInput.text);
        }
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.33000001311302185;height:480;width:640}
}
##^##*/
