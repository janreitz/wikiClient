#ifndef SQLQUERYCLIENT_H
#define SQLQUERYCLIENT_H

#include <QObject>

class SqlQueryClient : public QObject
{
    Q_OBJECT
public:
    explicit SqlQueryClient(QObject *parent = nullptr);

signals:

};

#endif // SQLQUERYCLIENT_H
