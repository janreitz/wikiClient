import QtQuick 2.15
import QtQuick.Shapes 1.15
import QtQuick.Controls 2.12
import "../components"
import Backend 1.0

Item {
    id: root

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
                startX: edge.ListView.view.originX + modelData.start.x
                startY: edge.ListView.view.originY + modelData.start.y
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
            x: ListView.view.originX + modelData.position.x - diameter/2
            y: ListView.view.originY + modelData.position.y - diameter/2
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
                onPressed: {
                    modelData.dragStarted()
                    timer.start()
                }
                onReleased: {
                    modelData.dragFinished()
                    timer.stop()
                }
                function setNodePositionToMouseCoordinates() {
                    modelData.position.x = modelData.position.x - node.diameter/2 + mouseX
                    modelData.position.y = modelData.position.y - node.diameter/2 + mouseY
                }

            }
        }
    }
}


