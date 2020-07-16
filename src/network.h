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
    QPoint discretizeNodePosition(const QPointF& nodePosition);

    Node* getOrCreateNode(const QString& nodeName, bool docExists);
    void internallyUpdateNodePosition(Node* node);

    QList<Node*> m_nodes;
    QHash<QString, Node*> m_nodesByName;
    QMultiHash<QPoint, Node*> m_nodesByPosition;
    QList<Edge*> m_edges;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    static constexpr float m_repellingConstant = 100000.0F;
    static constexpr float m_maxRepellingForce = 1000.0F;
    static constexpr float m_centerTetherSpringConstant = 100.0F;
    static constexpr float m_centerTetherDamperConstant = 0.0F;
    static constexpr float m_airFrictionConstant = 0.001F;
    static constexpr float m_gridSize = 33.0F;
};

#endif // NETWORK_H
