#include "network.h"
#include "utilities.h"
#include "performancesuite.h"

#include <QDebug>

Network::Network()
    : m_timer(this)
    , m_elapsedTimer()
{
    DBManager* theDBManager = DBManager::getInstance();
    if (theDBManager->isOpen())
        initializeNetwork();
    connect(theDBManager, &DBManager::signalDBInitialized, this, &Network::initializeNetwork);
    connect(&m_timer, &QTimer::timeout, this, &Network::tick);
    m_timer.start(stepSize * 1000);
}

void Network::initializeNetwork()
{
    clear();

    if (!m_dbOpen) {
        qWarning() << "Network::initializeNetwork -> DB not open";
        return;
    }

    // get all document names
    QSet<QString> docNames;
    m_query.exec("SELECT name FROM documents");
    while (m_query.next())
    {
        docNames << m_query.value(0).toString();
    }
    // create Edges
    m_query.exec("SELECT source, target FROM links WHERE type='INTERNAL'");
    while (m_query.next())
    {
        auto sourceName = m_query.value(0).toString();
        auto targetName = m_query.value(1).toString();
        bool sourceNodeDocExists =  docNames.contains(sourceName);
        bool targetNodeDocExists =  docNames.contains(targetName);
        m_edges << new Edge(getOrCreateNode(sourceName, sourceNodeDocExists),
                            getOrCreateNode(targetName, targetNodeDocExists),
                            this);
    }
}

void Network::nodePositionChanged()
{
    // Update position in m_nodesByPosition
    auto node = qobject_cast<Node*>(sender());
    if (node)
        internallyUpdateNodePosition(node);
}

void Network::internallyUpdateNodePosition(Node* node)
{
    const auto discretizedOldPosition = discretizeNodePosition(node->oldPosition());
    const auto discretizedNewPosition = discretizeNodePosition(node->position());
    if (discretizedNewPosition != discretizedOldPosition)
    {
        m_nodesByPosition.remove(discretizedOldPosition, node);
        m_nodesByPosition.insert(discretizedNewPosition, node);
    }
}

Node* Network::getOrCreateNode(const QString& nodeName, bool docExists)
{
    Q_ASSERT(!nodeName.isEmpty());
    if (m_nodesByName.contains(nodeName))
    {
        return m_nodesByName[nodeName];
    }
    else
    {
        auto node= new Node(QPointF(0,0), nodeName, docExists, this);
        m_nodes << node;
        emit nodesChanged();
        m_nodesByName[nodeName] = node;
        m_nodesByPosition.insert(discretizeNodePosition(node->position()), node);
        connect(node, &Node::positionChangedByUser, this, &Network::nodePositionChanged);
        return node;
    }
}

void Network::clear()
{
    m_nodes.clear();
    m_edges.clear();
    m_nodesByName.clear();
    emit nodesChanged();
    emit edgesChanged();
}

QQmlListProperty<Node> Network::nodes()
{
    return {this, this,
            &Network::nodeCount,
            &Network::nodeAt};
}

int Network::nodeCount() const
{
    return m_nodes.count();
}

Node* Network::nodeAt(int index) const
{
    return m_nodes.at(index);
}

QQmlListProperty<Edge> Network::edges()
{
    return {this, this,
            &Network::edgeCount,
            &Network::edgeAt};
}

int Network::edgeCount() const
{
    return m_edges.count();
}

Edge* Network::edgeAt(int index) const
{
    return m_edges.at(index);
}

void Network::tick()
{
    //m_elapsedTimer.start();
    PerformanceSuite::getInstance()->tick("Network::tick repelling");

    for (auto cell : m_nodesByPosition.uniqueKeys())
    {
        auto nodesInCell = m_nodesByPosition.values(cell);

        const auto neighboringCells = Utilities::getNeighboringCells(cell);
        QList<Node*> nodesInNeighboringCells;
        for (const auto neighboringCell : neighboringCells)
        {
            nodesInNeighboringCells += m_nodesByPosition.values(neighboringCell);
        }

        // IDEA Andere Zellen durch Schwerpunkt annähern
        auto relevantNodes = nodesInCell + nodesInNeighboringCells;

        for (auto i = 0; i < relevantNodes.count(); i++)
        {
            auto node_i = relevantNodes.at(i);
            const auto position_i = node_i->position();

            for (auto j = i; j < relevantNodes.count(); j++)
            {
                auto node_j = relevantNodes.at(j);
                const auto position_j = node_j->position();
                const auto d_ij = position_i - position_j;
                const auto length_d_ij = Utilities::vectorLength(d_ij);
                const auto d_norm_ij = Utilities::normalizeVector(d_ij);
                QPointF force;
                if (d_norm_ij) {
                    Q_ASSERT(length_d_ij != 0); // A result from normalizeVector implies non-zero length
                    force = *d_norm_ij * qMin(m_repellingConstant / (length_d_ij * length_d_ij), m_maxRepellingForce);
                } else {
                    force = m_maxRepellingForce * Utilities::randomNormalVector();
                }
                node_i->applyForce(force);
                node_j->applyForce(-force);
            }
        }
    }



// Jeder mit jedem
//    for (auto i = 0; i < m_nodes.count(); i++)
//    {
//        auto node_i = m_nodes.at(i);
//        const auto position_i = node_i->position();

//        for (auto j = i; j < m_nodes.count(); j++)
//        {
//            auto node_j = m_nodes.at(j);
//            const auto position_j = node_j->position();
//            const auto d_ij = position_i - position_j;
//            const auto length_d_ij = Utilities::vectorLength(d_ij);
//            const auto d_norm_ij = Utilities::normalizeVector(d_ij);
//            QPointF force;
//            if (d_norm_ij) {
//                Q_ASSERT(length_d_ij != 0); // A result from normalizeVector implies non-zero length
//                force = *d_norm_ij * qMin(m_repellingConstant / (length_d_ij * length_d_ij), m_maxRepellingForce);
//            } else {
//                force = m_maxRepellingForce * Utilities::randomNormalVector();
//            }
//            node_i->applyForce(force);
//            node_j->applyForce(-force);
//        }
//    }
    PerformanceSuite::getInstance()->tock("Network::tick repelling");

    PerformanceSuite::getInstance()->tick("Network::tick edgeforces");
    for (auto node : m_nodes)
    {
        node->applyEdgeForces();
        // tether nodes to the origin
        node->applyForce((-1) * Utilities::normalizeVectorOrRandomize(node->position()) * m_centerTetherSpringConstant);
        //node->applyForce((-1) * m_centerTetherDamperConstant * Utilities::vectorProjection(node->velocity(), node->position()));
        node->applyForce((-1) * m_airFrictionConstant * Utilities::vectorLength(node->velocity()) * node->velocity());
    }
    PerformanceSuite::getInstance()->tock("Network::tick edgeforces");
    PerformanceSuite::getInstance()->tick("Network::tick updates");
    for (auto node : m_nodes)
    {
        node->doStep();
        internallyUpdateNodePosition(node);
    }
    for (auto edge : m_edges)
    {
        edge->updatePositions();
    }
    PerformanceSuite::getInstance()->tock("Network::tick updates");
}

int Network::nodeCount(QQmlListProperty<Node>* list) {
    return reinterpret_cast< Network* >(list->data)->nodeCount();
}

Node* Network::nodeAt(QQmlListProperty<Node>* list, int index) {
    return reinterpret_cast< Network* >(list->data)->nodeAt(index);
}

int Network::edgeCount(QQmlListProperty<Edge>* list) {
    return reinterpret_cast< Network* >(list->data)->edgeCount();
}

Edge* Network::edgeAt(QQmlListProperty<Edge>* list, int index) {
    return reinterpret_cast< Network* >(list->data)->edgeAt(index);
}

QPoint Network::discretizeNodePosition(const QPointF &nodePosition)
{
    const int roundedX = round(nodePosition.x() / m_gridSize);
    const int roundedY = round(nodePosition.y() / m_gridSize);
    return QPoint(roundedX, roundedY);
}

