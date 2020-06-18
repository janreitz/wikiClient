#include "abstractdbclient.h"

AbstractDBClient::AbstractDBClient(DBManager* dbManager)
    : QObject(nullptr)
    , m_dbManager(dbManager)
{
    QObject::connect(m_dbManager, &DBManager::signalDBAvailable, this, [this]{
        m_dbOpen = true;
        if (auto q = m_dbManager->getQuery(); q)
            m_query = *q;
    });
    QObject::connect(m_dbManager, &DBManager::signalDBClosed, this, [this]{
        m_dbOpen = false;
        m_query.clear();
    });
}
