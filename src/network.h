#ifndef NETWORK_H
#define NETWORK_H

#include <QObject>
#include <QTimer>
#include <QQmlListProperty>
#include "node.h"
#include "edge.h"

class Network : public QObject
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

    explicit Network(QObject *parent = nullptr);

public slots:
    void tick();

signals:
    void nodesChanged() const;
    void edgesChanged() const;

private:

    static int nodeCount(QQmlListProperty<Node>*);
    static Node* nodeAt(QQmlListProperty<Node>*, int index);

    static int edgeCount(QQmlListProperty<Edge>*);
    static Edge* edgeAt(QQmlListProperty<Edge>*, int index);


    QList<Node*> m_nodes;
    QList<Edge*> m_edges;
    QTimer m_timer;
    double m_stepSize = 50;
};

#endif // NETWORK_H
