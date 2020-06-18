#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "node.h"
#include "edge.h"
#include "abstractdbclient.h"

class Network : public AbstractDBClient
{
    Q_OBJECT
    Q_PROPERTY(QQmlListProperty<Node> nodes READ nodes NOTIFY nodesChanged())
    Q_PROPERTY(QQmlListProperty<Edge> edges READ edges NOTIFY edgesChanged())

public:

    QQmlListProperty<Node> nodes();
    int nodeCount() const;
    Node* nodeAt(int index) const;

    QQmlListProperty<Edge> edges();
    int edgeCount() const;
    Edge* edgeAt(int index) const;

    explicit Network(DBManager* dbManager);

public slots:
    void tick();
    void initializeNetwork();

signals:
    void nodesChanged() const;
    void edgesChanged() const;

private:

    void clear();

    static int nodeCount(QQmlListProperty<Node>*);
    static Node* nodeAt(QQmlListProperty<Node>*, int index);

    static int edgeCount(QQmlListProperty<Edge>*);
    static Edge* edgeAt(QQmlListProperty<Edge>*, int index);

    QList<Node*> m_nodes;
    QHash<QString, Node*> m_nodesByName;
    QList<Edge*> m_edges;
    QTimer m_timer;
    double m_stepSize = 50;
};

#endif // NETWORK_H
