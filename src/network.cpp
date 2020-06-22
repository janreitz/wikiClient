#include "network.h"

Network::Network()
    : m_timer(this)
{
//    Node* node_1 = new Node(QPointF(200,400), "first", this);
//    Node* node_2 = new Node(QPointF(150,450), "second", this);
//    Node* node_3 = new Node(QPointF(200,500), "third", this);

//    Edge* edge_1 = new Edge(node_1, node_2, this);
//    Edge* edge_2 = new Edge(node_2, node_3, this);
//    Edge* edge_3 = new Edge(node_3, node_1, this);

//    m_nodes << node_1;
//    m_nodes << node_2;
//    m_nodes << node_3;

//    m_edges << edge_1;
//    m_edges << edge_2;
//    m_edges << edge_3;
    DBManager* theDBManager = DBManager::getInstance();
    if (theDBManager->isOpen())
        initializeNetwork();
    connect(theDBManager, &DBManager::signalDBInitialized, this, &Network::initializeNetwork);
    connect(&m_timer, &QTimer::timeout, this, &Network::tick);
    m_timer.start(m_stepSize);
}

void Network::initializeNetwork()
{
    if (!m_dbOpen) {
        qWarning() << "Network::initializeNetwork -> DB not open";
        return;
    }

    // get all document names
    QStringList docNames;
    m_query.exec("SELECT Name FROM Documents");
    // fill nodes
    while (m_query.next())
    {
        auto name = m_query.value(0).toString();
        Q_ASSERT(!name.isEmpty());
        auto node = new Node(QPointF(0,0), name, this);
        m_nodes << node;
        m_nodesByName[name] = node;
    }

    // get all links
    m_query.exec("SELECT Source, Target FROM Links");
    // fill edges
    while (m_query.next())
    {
        auto sourceName = m_query.value(0).toString();
        auto targetName = m_query.value(1).toString();
        if (m_nodesByName.contains(sourceName) && m_nodesByName.contains(targetName))
            m_edges << new Edge(m_nodesByName[sourceName], m_nodesByName[targetName], this);
    }
}

void Network::clear()
{
    m_nodes.clear();
    m_edges.clear();
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
    for (auto node : m_nodes)
    {
        node->calculateForces();
    }
    for (auto node : m_nodes)
    {
        node->updatePosition();
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

