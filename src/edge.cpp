#include "edge.h"
#include <QDebug>

Edge::Edge(Node* source, Node* target, QObject* parent)
    : QObject(parent)
    , m_source(source)
    , m_target(target)
{
    m_source->addEdge(this);
    m_target->addEdge(this);

    m_length = calcLength();
    m_oldLength = m_length;
    m_d_length_dt = 0;

    QObject::connect(m_source, &Node::positionChanged, this, &Edge::slotSourcePositionChanged);
    QObject::connect(m_target, &Node::positionChanged, this, &Edge::slotTargetPositionChanged);
}

Edge::~Edge()
{
    //qDebug() << "Edge::~Edge";
}

QPointF Edge::start() const
{
    return m_start;
}

QPointF Edge::end() const
{
    return m_end;
}

Node* Edge::node1() const
{
    return m_source;
}

Node* Edge::node2() const
{
    return m_target;
}

void Edge::slotSourcePositionChanged()
{
    m_start = m_source->position();
    emit startChanged();
}

void Edge::slotTargetPositionChanged()
{
    m_end = m_target->position();
    emit endChanged();
}

double Edge::calcLength() const
{
    QPointF delta = m_target->position() - m_source->position();
    return sqrt(pow(delta.x(), 2) + pow(delta.y(), 2));
}

double Edge::force() const
{
    double springForce = (m_length - m_neutralLength) * m_springConstant;
    // This is a problem, since the sampling rate is unkown
    double dampingForce = (m_length - m_oldLength) / (m_stepSize / 1000) * m_dampingConstant;
    return springForce + dampingForce;
}

QPointF Edge::normalizedVector() const
{
    return (m_target->position() - m_source->position()) / m_length;
}

void Edge::updatePositions()
{
    m_start = m_source->position();
    m_end = m_target->position();

    m_oldLength = m_length;
    m_length = calcLength();
    emit startChanged();
    emit endChanged();
}


