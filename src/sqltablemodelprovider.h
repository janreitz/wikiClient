#ifndef SQLTABLEMODELPROVIDER_H
#define SQLTABLEMODELPROVIDER_H

#include <QObject>
#include <QtSql/QSqlTableModel>

#include "dbmanager.h"

class SqlTableModelProvider : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QSqlTableModel* model READ model NOTIFY modelChanged)

public:
    SqlTableModelProvider(DBManager* dbManager, QObject* parent = nullptr);

    bool acquireModel();
    QSqlTableModel* model();
    bool setTable(const QString& tableName);

signals:
    void modelChanged();

private:

    QSqlTableModel* m_tableModel;
    DBManager* m_dbManager;
};

#endif // SQLTABLEMODELPROVIDER_H
