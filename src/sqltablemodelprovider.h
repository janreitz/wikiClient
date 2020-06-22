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
    SqlTableModelProvider(QObject* parent = nullptr);

    QSqlTableModel* model();
    void setTable(const QString& tableName);

public slots:

    void slotDBOpened();

signals:
    void modelChanged();

private:
    void getTableModel();
    std::optional<std::shared_ptr<QSqlTableModel>> m_tableModel;
    DBManager* m_dbManager;
};

#endif // SQLTABLEMODELPROVIDER_H
