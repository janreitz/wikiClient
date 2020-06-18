#include "node.h"
#include "edge.h"

#include <QDebug>

Node::Node(const QPointF& position , const QString& name, QObject *parent)
    : QObject(parent)
    , m_position(position)
    , m_position_t_plus_1(position)
    , m_name(name)
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

void Node::setPosition(const QPointF& pos)
{
    if (isnan(pos.x()) || isnan(pos.y()))
    {
        qDebug() << "Node::setPosition -> position less than zero";
    }
    m_position = pos;
    m_position_t_plus_1 = pos;
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
    Q_ASSERT(!isnan(resultingEdgeForce.x()) && !isnan(resultingEdgeForce.y()));
    // TODO Figure out a smart way to access relevant other nodes to add repelling forces.
    m_position_t_plus_1 = m_position + resultingEdgeForce;
}

void Node::updatePosition()
{
    if (m_isBeingDragged)
        return;

    setPosition(m_position_t_plus_1);
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
