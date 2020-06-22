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
        MatchContextRole,
        NameRole
    };

    Q_INVOKABLE void fullTextSearch(const QString& searchText);

    SearchBackend();

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QHash<int, QByteArray> roleNames() const override;
    Qt::ItemFlags flags(const QModelIndex &index) const override;

private:

    DBManager* m_dbManager;
    QSqlQuery mQuery;
    bool mDBOpen;
    QList<SearchResult> m_searchResults;
};

#endif // SEARCHBACKEND_H
