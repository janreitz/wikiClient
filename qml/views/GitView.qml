import QtQuick 2.0
import QtQuick.Controls 2.15

Item {
    id: root

    Column {
        Button{
            id: addAllMdButton
            text: "Add all .md files"
            onPressed: theGitManager.addAll();
        }
        Button {
            id: commitButton
            text: "Commit"
            onPressed: theGitManager.commit("I committed from the GUI");
        }
    }
}
