#ifndef NODE_H
#define NODE_H

#include <QObject>

class Edge;
class Node : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF position READ position WRITE setPosition NOTIFY positionChanged);
    Q_PROPERTY(QString name READ name NOTIFY nameChanged)

public:
    explicit Node(const QPointF& position = QPointF(0,0),
                  const QString& name = "",
                  QObject *parent = nullptr);
    ~Node();

    bool operator==(const Node& other);

    QPointF position() const;
    void setPosition(const QPointF& x);

    QString name() const;

    void addEdge(Edge* edge);
    void applyEdgeForces();
    void applyForce(const QPointF& force);
    void updatePosition();

public slots:
    void dragStarted();
    void dragFinished();

signals:
    void positionChanged();
    void nameChanged();

private:
    bool m_isBeingDragged = false;
    QPointF m_position;
    QPointF m_currentForce;
    QString m_name;
    QList<Edge*> m_edges;
};

uint qHash(const Node& node);

#endif // NODE_H
