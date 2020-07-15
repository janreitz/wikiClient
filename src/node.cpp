#include "node.h"
#include "edge.h"
#include "network.h"

#include <QDebug>

Node::Node(const QPointF& position , const QString& name, const bool& docExists, QObject *parent)
    : QObject(parent)
    , m_position(position)
    , m_velocity(0,0)
    , m_currentForce(0,0)
    , m_name(name)
    , m_docExists(docExists)
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

bool Node::docExists() const
{
    return m_docExists;
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
    applyForce(resultingEdgeForce);
}

void Node::applyForce(const QPointF &force)
{
    Q_ASSERT(!isnan(force.x()) && !isnan(force.y()));
    m_currentForce += force;
}

void Node::doStep()
{
    if (m_isBeingDragged)
        return;

    const QPointF acceleration = m_currentForce / m_mass;
    m_velocity += acceleration * Network::stepSize;
    setPosition(m_position + m_velocity * Network::stepSize);
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
