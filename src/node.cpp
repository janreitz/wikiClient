#include "node.h"
#include "edge.h"

#include <QDebug>

Node::Node(const QPointF& position , const QString& name, QObject *parent)
    : QObject(parent)
    ,m_position(position)
    ,m_name(name)
{

}

Node::~Node()
{
    //qDebug() << "Node::~Node -> " + m_name;
}

bool Node::operator==(const Node &other)
{
    return m_name == other.name();
}

QPointF Node::position() const
{
    return m_position;
}

void Node::setPosition(const QPointF& position)
{
    m_position = position;
    m_position_t_plus_1 = position;
    emit positionChanged();
}


QString Node::name() const
{
    return m_name;
}

void Node::addEdge(Edge *edge)
{
    if (edge)
    {
        m_edges << edge;
    }
}

void Node::calculateForces()
{
    QPointF resultingEdgeForce(0,0);
    for (auto edge: m_edges)
    {
        if (this == edge->node1())
        {
            resultingEdgeForce += (edge->normalizedVector() * edge->force());
        }
        else if (this == edge->node2())
        {
            resultingEdgeForce -= (edge->normalizedVector() * edge->force());
        }
    }
    // TODO Figure out a smart way to access relevant other nodes to add repelling forces.

    m_position_t_plus_1 = m_position + resultingEdgeForce;
}

void Node::updatePosition()
{
    if (m_isBeingDragged)
        return;

    m_position = m_position_t_plus_1;
    emit positionChanged();
}

void Node::dragStarted()
{
    m_isBeingDragged = true;
}

void Node::dragFinished()
{
    m_isBeingDragged = false;
}

uint qHash(const Node &node)
{
    return qHash(node.name());
}
