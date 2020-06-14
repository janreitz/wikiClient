#include "searchbackend.h"

SearchBackend::SearchBackend(DBManager* dbManager)
    : m_dbManager(dbManager)
{
    QObject::connect(m_dbManager, &DBManager::signalDBAvailable, this, [this]{
        mDBOpen = true;
        if (auto q = m_dbManager->getQuery(); q)
            mQuery = *q;
    });
    QObject::connect(m_dbManager, &DBManager::signalDBClosed, this, [this]{
        mDBOpen = false;
        mQuery.clear();
    });

    m_searchResults << SearchResult("Title 1", "... Lorem <b>ipsum dolor sit</b> amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et ...")
                    << SearchResult("Title 2", "... Lorem <b>ipsum dolor sit</b> amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et ...")
                    << SearchResult("Title 3", "... Lorem <b>ipsum dolor sit</b> amet, consetetur sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore magna aliquyam erat, sed diam voluptua. At vero eos et accusam et ...");
}

int SearchBackend::rowCount(const QModelIndex &) const
{
    return m_searchResults.count();
}

QVariant SearchBackend::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case TitleRole: return m_searchResults.at(index.row()).title();
        case MatchContextRole: return m_searchResults.at(index.row()).matchContext();
        default: return QVariant();
    }
    return QVariant();
}

QHash<int, QByteArray> SearchBackend::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[TitleRole] = "title";
    roles[MatchContextRole] = "matchContext";
    return roles;
}

Qt::ItemFlags SearchBackend::flags(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return Qt::ItemIsSelectable;
}

void SearchBackend::fullTextSearch(const QString& searchText)
{
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return;
    }

    QString searchQuery("SELECT Title, highlight(fts_Documents, 1, '<b>', '</b>') FROM fts_Documents WHERE Content MATCH '" + searchText + "';");

    if (!mQuery.exec(searchQuery))
    {
        qDebug() << "DBManager::createNewDB -> Query failed: " << searchQuery
                 << " with Error: " << mQuery.lastError();
        return;
    }

    beginRemoveRows(QModelIndex(), 0, m_searchResults.count() - 1);

    m_searchResults.clear();

    endRemoveRows();

    QList<SearchResult> intermediateSearchresults;
    while (mQuery.next())
    {
        QString title;
        QString matchContext;

        intermediateSearchresults << SearchResult(mQuery.value(0).toString(), mQuery.value(1).toString());
    }
    beginInsertRows(QModelIndex(), 0, intermediateSearchresults.count());
    m_searchResults = intermediateSearchresults;

    endInsertRows();
}
