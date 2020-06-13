#ifndef SEARCHBACKEND_H
#define SEARCHBACKEND_H

#include <QAbstractListModel>

#include "dbmanager.h"
#include "searchresult.h"

class SearchBackend : public QAbstractListModel
{
    Q_OBJECT

public:

    enum SearchResultRoles {
        TitleRole = Qt::UserRole + 1,
        MatchContextRole
    };

    Q_INVOKABLE void fullTextSearch(const QString& searchText);

    SearchBackend(DBManager* dbManager);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QHash<int, QByteArray> roleNames() const;

private:

    DBManager* m_dbManager;
    QSqlQuery mQuery;
    bool mDBOpen;
    QList<SearchResult> m_searchResults;
};

#endif // SEARCHBACKEND_H
