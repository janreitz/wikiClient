#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTimer>
#include <QElapsedTimer>
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

    explicit Network();

    static constexpr double stepSize = 0.05; //seconds

public slots:
    void tick();
    void initializeNetwork();
    void nodePositionChanged();

signals:
    void nodesChanged() const;
    void edgesChanged() const;

private:

    void clear();
    static int nodeCount(QQmlListProperty<Node>*);
    static Node* nodeAt(QQmlListProperty<Node>*, int index);
    static int edgeCount(QQmlListProperty<Edge>*);
    static Edge* edgeAt(QQmlListProperty<Edge>*, int index);
    QPointF discretizeNodePosition(const QPointF& nodePosition);

    Node* getOrCreateNode(const QString& nodeName, bool docExists);

    QList<Node*> m_nodes;
    QHash<QString, Node*> m_nodesByName;
    QMultiHash<QPointF, Node*> m_nodesByPosition;
    QList<Edge*> m_edges;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    static constexpr double m_repellingConstant = 100000;
    static constexpr double m_maxRepellingForce = 1000;
    static constexpr double m_centerTetherSpringConstant = 100;
    static constexpr double m_centerTetherDamperConstant = 0;
    static constexpr double m_airFrictionConstant = 0.001;
    static constexpr double m_gridSize = 300;
};

#endif // NETWORK_H
