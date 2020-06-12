import QtQuick 2.15
import QtQuick.Controls 2.15

ItemDelegate {

    states: [
        State {
            name: "expanded"
            when: delegate.checked

            PropertyChanges {
                target: grid
                visible: true
            }
        }
    ]
}
