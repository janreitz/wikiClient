#include "edge.h"
#include <QDebug>
#include <QRandomGenerator>

Edge::Edge(Node* source, Node* target, QObject* parent)
    : QObject(parent)
    , m_source(source)
    , m_target(target)
{
    Q_ASSERT(m_source);
    Q_ASSERT(m_target);
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
    const double length = vectorLength(delta);
    Q_ASSERT(!isnan(length));
    return length;
}

double Edge::vectorLength(const QPointF & vec)
{
    return sqrt(pow(vec.x(), 2) + pow(vec.y(), 2));
}

std::optional<QPointF> Edge::normalizeVector(const QPointF & vec)
{
    const double length = vectorLength(vec);
    if (length == 0)
        return std::nullopt;
    return vec/vectorLength(vec);
}

double Edge::force() const
{
    const double springForce = (m_length - m_neutralLength) * m_springConstant;
    // This is a problem, since the sampling rate is unkown
    const double dampingForce = (m_length - m_oldLength) / (m_stepSize / 1000) * m_dampingConstant;
    const double resultingForce = springForce + dampingForce;
    Q_ASSERT(!isnan(resultingForce));
    return resultingForce;
}

QPointF Edge::normalizedVector() const
{
    const auto normVector = normalizeVector(m_target->position() - m_source->position());
    if (normVector)
        return *normVector;
    // In case source and target have the same position, generate a random vector
    int randX = QRandomGenerator::global()->bounded(100);
    const int randY = QRandomGenerator::global()->bounded(100);
    if (randX == 0 && randY == 0)
    {
        randX = 1;
    }
    return *normalizeVector(QPointF(randX, randY));
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


