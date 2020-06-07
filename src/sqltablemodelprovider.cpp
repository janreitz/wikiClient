#include "sqltablemodelprovider.h"

SqlTableModelProvider::SqlTableModelProvider(DBManager* dbManager, QObject* parent)
    : QObject(parent)
    , m_tableModel(nullptr)
    , m_dbManager(dbManager)
{
}

bool SqlTableModelProvider::acquireModel()
{
    //m_tableModel = m_dbManager->getTableModel();
    return false;
}

QSqlTableModel* SqlTableModelProvider::model() {
    return m_tableModel;
}


