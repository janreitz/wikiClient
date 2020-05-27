import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.13
import QtQuick.Layouts 1.3


Flickable {
    id: flickable
    flickableDirection: Flickable.VerticalFlick
    Layout.fillHeight: true
    Layout.fillWidth: true

    TextArea.flickable: TextArea {
        id: textArea
        height: parent.height
        width: parent.width
        background: Rectangle {
            color: "lightblue"
        }
        textFormat: Qt.RichText
        wrapMode: TextArea.Wrap
        selectByMouse: true
        persistentSelection: true
        leftPadding: 6
        rightPadding: 6
        topPadding: 6
        bottomPadding: 6
        placeholderText: "Your Awesome Wiki Article"
        activeFocusOnTab: true
        onActiveFocusChanged: {
            if (activeFocus)
                console.log("Editor::" + objectName + "::onActiveFocusChanged -> received active focus");
            else
                console.log("Editor::" + objectName + "::onActiveFocusChanged -> lost active focus");
        }
    }

    Shortcut {
        sequence: "Ctrl+M"
        onActivated: {
            textArea.textFormat = textArea.textFormat == TextEdit.MarkdownText ? TextEdit.AutoText : TextEdit.MarkdownText;
        }
    }
    // SneakPreview
}

/*##^##
Designer {
    D{i:0;autoSize:true;formeditorZoom:0.8999999761581421;height:480;width:640}
}
##^##*/
