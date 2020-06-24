#include "node.h"
#include "edge.h"
#include "network.h"

#include <QDebug>

Node::Node(const QPointF& position , const QString& name, QObject *parent)
    : QObject(parent)
    , m_position(position)
    , m_currentForce()
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
    Q_ASSERT(!isnan(pos.x()) | !isnan(pos.y()));
    m_position = pos;
    m_currentForce = QPointF();
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

void Node::applyEdgeForces()
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
    m_currentForce += resultingEdgeForce;
}

void Node::applyForce(const QPointF &force)
{
    Q_ASSERT(!isnan(force.x()) && !isnan(force.y()));
    m_currentForce += force;
}

void Node::updatePosition()
{
    if (m_isBeingDragged)
        return;

    setPosition(m_position + m_currentForce * Network::stepSize);
    m_currentForce = QPointF();
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
