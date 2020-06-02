import QtQuick 2.15
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

Rectangle {
    id: root
    visible: true
    property alias queryInput: searchBarInput
    signal signalSearch(string searchString)
    color: theme.colorAreaBackground

    FocusScope {
        id: focusScope
        anchors.fill: parent

        TextField {
            id: searchBarInput
            focus: true
            objectName: "searchBarInput"
            anchors.left: parent.left
            anchors.leftMargin: 20
            anchors.right: searchTypeComboBox.left
            anchors.rightMargin: 20
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            selectByMouse: true
            width: root.width * 0.7
            leftPadding: 10
            font: theme.fontSideBarNormal
            color: theme.colorTextLight
            verticalAlignment: Text.AlignVCenter
            background: Rectangle {
                radius: 10
                color: theme.colorAreaHighlight
            }
            Keys.onPressed: {
                switch (event.key)
                {
                case Qt.Key_Down:
                    suggestionBox.focus = true
                    break;
                case Qt.Key_Return:
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
            enabled: searchBarInput.activeFocus
            anchors.top: searchBarInput.bottom
            anchors.left: searchBarInput.left
            anchors.right: searchBarInput.right
            suggestionHeight: searchBarInput.height
            maxHeight: 400
            onSignalSuggestionAccepted: {
                queryInput.text = suggestion;
                searchBarInput.focus = true
            }
            onSignalExitedAbove: {
                searchBarInput.focus = true
            }
        }

        ComboBox {
            id: searchTypeComboBox
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            anchors.right: searchButton.left
            width: 100
            model: ["Full text", "Titles", "SQL"]
        }

        Button {
            id: searchButton
            text: "Go"
            anchors.right: loadDemoDirButton.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 50
            onClicked: doSearch()
            Keys.onReturnPressed: doSearch()
        }

        Button {
            id: loadDemoDirButton
            text: "LoadDemoDir"
            anchors.right: parent.right
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: 125
            onClicked: loadDemoDir()
            Keys.onReturnPressed: loadDemoDir()
        }

    }

    function loadDemoDir () {
        theFileManager.slotSetDirectory("C:/Users/jan-r/projects/wikiClient/demo");
        theFileManager.slotScanDirectory();
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
