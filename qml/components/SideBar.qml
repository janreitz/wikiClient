import QtQuick 2.15
import QtQuick.Controls 2.12
import "../views"

Item {
    id: root
    width: childrenRect.width

    property var lastActiveEditor
    property var lastActiveMultiTabEditor

    Rectangle {
        id: permanentSidebar
        color: "#1E1E1E"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.bottom: parent.bottom
        width: 50

        Column {
            id: col
            spacing: 20
            anchors.horizontalCenter: permanentSidebar.horizontalCenter
            anchors.top: permanentSidebar.top
            anchors.topMargin: 20
            IconButton {
                id: filesButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/files_outline.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (fileViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        fileViewLoader.sourceComponent = fileViewComp;
                    } else {
                        root.sideBarButtonPress(fileViewLoader.item)
                    }
                }
            }
            IconButton {
                id: linksButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/link.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (linkViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        linkViewLoader.sourceComponent = linkViewComp;
                    } else {
                        root.sideBarButtonPress(linkViewLoader.item)
                    }
                }
            }
            IconButton {
                id: searchButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/search.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (searchViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        searchViewLoader.sourceComponent = searchViewComp;
                    } else {
                        root.sideBarButtonPress(searchViewLoader.item)
                    }
                }
            }
            IconButton {
                id: networkButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/network.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (networkViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        networkViewLoader.sourceComponent = networkViewComp;
                    } else {
                        root.sideBarButtonPress(networkViewLoader.item)
                    }
                }
            }
            IconButton {
                id: sqlButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/database.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (sqlViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        sqlViewLoader.sourceComponent = sqlViewComp;
                    } else {
                        root.sideBarButtonPress(sqlViewLoader.item)
                    }
                }
            }
            IconButton {
                id: gitButton
                anchors.horizontalCenter: parent.horizontalCenter
                width: permanentSidebar.width * 0.7
                height: width
                source: "qrc:/resources/icons/git.svg"
                onPressed: {
                    // Is my item already loaded?
                    if (gitViewLoader.status !== Loader.Ready) {
                        // the loader will "press the button" after loading
                        gitViewLoader.sourceComponent = gitViewComp;
                    } else {
                        root.sideBarButtonPress(gitViewLoader.item)
                    }
                }
            }
        }

        IconButton {
            id: settingsButton
            anchors.horizontalCenter: permanentSidebar.horizontalCenter
            width: permanentSidebar.width * 0.7
            height: width
            anchors.bottom: permanentSidebar.bottom
            anchors.bottomMargin: 20
            source: "qrc:/resources/icons/gear_filled.svg"
            onPressed: {
                root.lastActiveMultiTabEditor.openFileInNewTab(":/resources/settings/default.json");
            }
        }
    }

    SlideOutRectangle {
        id: slideOutSidebar
        anchors.left: permanentSidebar.right
        anchors.top: permanentSidebar.top
        anchors.bottom: permanentSidebar.bottom
        backgroundColor: "#2d2d2d"
        state: "slideIn"
        slideoutWidth: 600
    }

    // I want to load views the first time someone clicks the corresponding button

    Loader {
        id: fileViewLoader
        onLoaded: {
            console.log("fileViewLoader -> loaded")
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: fileViewComp
        FileView {
            id: fileView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "fileView"
            lastActiveEditor: root.lastActiveEditor
        }
    }

    Loader {
        id: linkViewLoader
        onLoaded: {
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: linkViewComp
        LinkView {
            id: linkView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "linkView"
            lastActiveEditor: root.lastActiveEditor
        }
    }

    Loader {
        id: searchViewLoader
        onLoaded: {
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: searchViewComp
        SearchView {
            id: searchView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "searchView"
            lastActiveEditor: root.lastActiveEditor
            lastActiveMultiTabEditor: root.lastActiveMultiTabEditor
        }
    }

    Loader {
        id: networkViewLoader
        onLoaded: {
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: networkViewComp
        NetworkView {
            id: networkView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "networkView"
            lastActiveEditor: root.lastActiveEditor
            lastActiveMultiTabEditor: root.lastActiveMultiTabEditor
        }
    }

    Loader {
        id: sqlViewLoader
        onLoaded: {
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: sqlViewComp
        SqlView {
            id: sqlView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "sqlView"
        }
    }

    Loader {
        id: gitViewLoader
        onLoaded: {
            root.sideBarButtonPress(item)
        }
    }

    Component {
        id: gitViewComp
        GitView {
            id: gitView
            anchors.fill: parent
            anchors.margins: 20
            objectName: "gitView"
        }
    }
    function sideBarButtonPress (buttonContentItem) {
        if (slideOutSidebar.contentItem) {
            // Is it me?
            if (slideOutSidebar.contentItem.objectName === buttonContentItem.objectName) {
                slideOutSidebar.toggleSlideout();
                return;
            }
        }
        // The slidebar has no content, or someone elses
        slideOutSidebar.setContentItem(buttonContentItem);
        slideOutSidebar.slideout()
    }
}





