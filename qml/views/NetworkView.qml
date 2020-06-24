import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.12
import "../components"
import Backend 1.0

Flickable {
    id: root
    objectName: "NetworkView"
    contentWidth: nodeList.width
    contentHeight: nodeList.height

    property var lastActiveEditor
    property var lastActiveMultiTabEditor

    Item {
        id: contentRoot
        objectName: "NetworkView"

        Network {
            id: network
            objectName: root.objectName + "::network"
            Component.onCompleted: console.log(objectName + " -> completed")
        }

        ListView {
            id: edgeList
            model: network.edges
            delegate: edgeComp
            interactive: false
            clip: true
        }

        Component {
            id: edgeComp
            Shape {
                id: edge
                width: childrenRect.width
                height: childrenRect.height
                ShapePath{
                    id: shapePath
                    startX: modelData.start.x
                    startY: modelData.start.y
                    strokeColor: "#ccc"
                    strokeWidth: 2
                    PathLine {
                        id: edgeLine
                        x: modelData.end.x
                        y: modelData.end.y
                    }
                }
            }
        }

        ListView {
            id: nodeList
            objectName: "nodeList"
            width: contentWidth
            height: contentHeight
            model: network.nodes
            delegate: nodeComp
            interactive: false
            clip: true
        }

        Component {
            id: nodeComp
            Item {
                id: nodeRoot
                Rectangle {
                    property var diameter: 20
                    id: node
                    objectName: "node"
                    x: modelData.position.x - diameter/2
                    y: modelData.position.y - diameter/2
                    color: mouseArea.containsMouse ? "#ffcc66" : "#99cc99"
                    width: diameter
                    height: diameter
                    radius: diameter / 2

                    Text {
                        id: nameDisplay
                        x: 30
                        font: theme.fontTextBody
                        color: theSettings.colorTextLight
                        text: modelData.name
                        visible: mouseArea.containsMouse
                    }

                    Timer {
                        id: timer
                        interval: 50
                        repeat: true
                        onTriggered: {
                            mouseArea.setNodePositionToMouseCoordinates()
                        }
                    }

                    MouseArea {
                        id: mouseArea
                        anchors.fill: parent
                        hoverEnabled: true
                        acceptedButtons: Qt.LeftButton | Qt.RightButton
                        onPressed: {
                            if (mouse.button === Qt.LeftButton) {
                                modelData.dragStarted()
                                timer.start()
                            }
                        }
                        onReleased: {
                            if (mouse.button === Qt.RightButton){
                                contextMenu.popup()
                            } else {
                            modelData.dragFinished()
                            timer.stop()
                            }
                        }

                        function setNodePositionToMouseCoordinates() {
                            modelData.position.x = modelData.position.x - node.diameter/2 + mouseX
                            modelData.position.y = modelData.position.y - node.diameter/2 + mouseY
                        }

                        Menu {
                            id: contextMenu
                            Action {
                                text: "Open in current tab"
                                onTriggered: {
                                    root.lastActiveEditor.loadFileRelative(modelData.name + ".md")
                                }
                            }
                            Action {
                                text: "Open in new tab"
                                onTriggered: {
                                    var newEditor = root.lastActiveMultiTabEditor.addNewTab();
                                    newEditor.loadFileRelative(modelData.name + ".md")
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
