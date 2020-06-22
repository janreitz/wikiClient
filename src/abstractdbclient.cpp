#include "abstractdbclient.h"

AbstractDBClient::AbstractDBClient()
    : QObject(nullptr)
    , m_dbManager(DBManager::getInstance())
{
    QObject::connect(m_dbManager, &DBManager::signalDBAvailable, this, [this]{
        m_dbOpen = true;
        if (auto q = m_dbManager->getQuery(); q)
            m_query = *q;
        emit queryAvailable();
    });
    QObject::connect(m_dbManager, &DBManager::signalDBClosed, this, [this]{
        m_dbOpen = false;
        m_query.clear();
        emit queryUnavailable();
    });
}
