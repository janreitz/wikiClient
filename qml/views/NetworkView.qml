import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.12

import Backend 1.0

Item {
    id: root
    property int diameter: 20
    property var springConstant: 0.1
    property var dampingConstant: 0.01
    property int stepSize: 20
    property int neutralLength: 100

    ListView {
        id: edgeList
        anchors.fill: parent
        model: theNetwork.edges
        delegate: edgeComp
        interactive: false
    }

    Component {
        id: edgeComp
        Shape {
            id: edge
            ShapePath{
                id: shapePath
                startX: modelData.start.x
                startY: modelData.start.y
                strokeColor: "black"
                strokeWidth: 5
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
        anchors.fill: parent
        model: theNetwork.nodes
        delegate: nodeComp
        interactive: false
    }

    Component {
        id: nodeComp
        Rectangle {
            property var diameter: 20
            id: node
            objectName: "node"
            x: modelData.position.x - diameter/2
            y: modelData.position.y - diameter/2
            color: "red"
            width: diameter
            height: diameter
            radius: diameter / 2

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
                onPressed: {
                    modelData.dragStarted()
                    timer.start()
                }
                onReleased: {
                    modelData.dragFinished()
                    timer.stop()
                }
                function setNodePositionToMouseCoordinates() {
                    modelData.position.x = node.ListView.view.x + node.x + mouseX
                    modelData.position.y = node.ListView.view.y + node.y + mouseY
                }

            }
        }
    }
}


