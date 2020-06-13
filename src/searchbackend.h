#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QAbstractListModel>

#include "dbmanager.h"
#include "searchresult.h"

class SearchBackend : public QAbstractListModel
{
    Q_OBJECT

public:

    Q_INVOKABLE void fullTextSearch(const QString& searchText);

    SearchBackend(DBManager* dbManager);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation,
                                int role = Qt::DisplayRole) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;

private:

    DBManager* m_dbManager;
    QSqlQuery mQuery;
    bool mDBOpen;
    QList<SearchResult> m_searchResults;
};

#endif // SEARCHBACKEND_H
