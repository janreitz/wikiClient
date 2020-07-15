#include "network.h"
#include "utilities.h"

#include <QDebug>

Network::Network()
    : m_timer(this)
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
    for (auto i = 0; i < m_nodes.count(); i++)
    {
        auto node_i = m_nodes.at(i);
        const auto position_i = node_i->position();

        for (auto j = i; j < m_nodes.count(); j++)
        {
            auto node_j = m_nodes.at(j);
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
    for (auto node : m_nodes)
    {
        node->applyEdgeForces();
        // tether nodes to the origin
        node->applyForce((-1) * Utilities::normalizeVectorOrRandomize(node->position()) * m_centerTetherSpringConstant);
    }
    for (auto node : m_nodes)
    {
        node->doStep();
    }
    for (auto edge : m_edges)
    {
        edge->updatePositions();
    }
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

