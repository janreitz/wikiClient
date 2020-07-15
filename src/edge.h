#ifndef EDGE_H
#define EDGE_H

#include <QObject>
#include <optional>

#include "node.h"

class Edge : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QPointF start READ start NOTIFY startChanged);
    Q_PROPERTY(QPointF end READ end NOTIFY endChanged);

public:
    QPointF start() const;
    QPointF end() const;
    Node* node1() const;
    Node* node2() const;

    explicit Edge(Node* source = nullptr,
                  Node* target = nullptr,
                  QObject* parent = nullptr);
    ~Edge();

    double force() const;
    QPointF normalizedVector() const;
    void updatePositions();

public slots:
    void slotSourcePositionChanged();
    void slotTargetPositionChanged();

signals:
    void startChanged();
    void endChanged();

private:
    double calcLength() const;

    double m_length, m_oldLength;
    double m_d_length_dt;
    static constexpr double m_springConstant = 5;
    static constexpr double m_dampingConstant = 1;
    static constexpr double m_neutralLength = 100;
    QPointF m_start, m_end;
    Node* m_source,* m_target;
};

#endif // EDGE_H
