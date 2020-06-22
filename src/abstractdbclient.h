#ifndef ABSTRACTSUGGESTIONPROVIDER_H
#define ABSTRACTSUGGESTIONPROVIDER_H

#include <QObject>
#include <QSqlQueryModel>

#include "dbmanager.h"
#include "suggestion.h"

class AbstractDBClient : public QObject
{
    Q_OBJECT
public:
    explicit AbstractDBClient();

signals:
    void queryAvailable();
    void queryUnavailable();

protected:
    DBManager* m_dbManager;
    QSqlQuery m_query;
    bool m_dbOpen;
};

#endif // ABSTRACTSUGGESTIONPROVIDER_H
