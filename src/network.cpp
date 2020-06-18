#include "network.h"

Network::Network(DBManager* dbManager)
    : AbstractDBClient(dbManager)
    , m_timer(this)
{
    connect(&m_timer, &QTimer::timeout, this, &Network::tick);
    m_timer.start(m_stepSize);

    Node* node_1 = new Node(QPointF(100,100), "first", this);
    Node* node_2 = new Node(QPointF(150,150), "second", this);
    Node* node_3 = new Node(QPointF(200,100), "third", this);

    Edge* edge_1 = new Edge(node_1, node_2, this);
    Edge* edge_2 = new Edge(node_2, node_3, this);
    Edge* edge_3 = new Edge(node_3, node_1, this);

    m_nodes << node_1;
    m_nodes << node_2;
    m_nodes << node_3;

    m_edges << edge_1;
    m_edges << edge_2;
    m_edges << edge_3;
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

