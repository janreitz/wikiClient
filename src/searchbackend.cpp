#include "searchbackend.h"

SearchBackend::SearchBackend(DBManager* dbManager)
    : m_dbManager(dbManager)
{
    QObject::connect(m_dbManager, &DBManager::signalDBOpened, this, [this]{
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

void SearchBackend::fullTextSearch(const QString& searchText)
{
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return;
    }


//    if (!mQuery.exec("SELECT Title, Content From Documents Where Content Like '" + docTitle + "');")
//        return;)

    m_searchResults.clear();


    while (mQuery.next())
    {
        QString title;
        QString matchContext;

        m_searchResults << SearchResult();
    }

    emit dataChanged(index(0), index(rowCount()));
}
