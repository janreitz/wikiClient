#include "sqltablemodelprovider.h"

SqlTableModelProvider::SqlTableModelProvider(QObject* parent)
    : QObject(parent)
    , m_dbManager(DBManager::getInstance())
{

}

QSqlTableModel* SqlTableModelProvider::model() {
    if (!m_tableModel)
        return nullptr;
    return (*m_tableModel).get();
}

void SqlTableModelProvider::setTable(const QString &tableName)
{
    if (!m_tableModel)
        return;

    (*m_tableModel)->setTable(tableName);
    (*m_tableModel)->select();
}

void SqlTableModelProvider::slotDBOpened()
{
    getTableModel();
    setTable("Documents");
}

void SqlTableModelProvider::getTableModel()
{
    auto tableModelOpt = m_dbManager->tableModel();

    if (tableModelOpt) {
        m_tableModel = *tableModelOpt;
        emit modelChanged();
    }
}


