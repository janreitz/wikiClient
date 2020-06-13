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
}

int SearchBackend::rowCount(const QModelIndex &) const
{
    return m_searchResults.count();
}

QVariant SearchBackend::data(const QModelIndex &index, int role) const
{
//    if (index.row() < rowCount())
//        switch (role) {
//        case FullNameRole: return m_contacts.at(index.row()).fullName;
//        case AddressRole: return m_contacts.at(index.row()).address;
//        case CityRole: return m_contacts.at(index.row()).city;
//        case NumberRole: return m_contacts.at(index.row()).number;
//        default: return QVariant();
//    }
    return QVariant();
}

QVariant SearchBackend::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
}

Qt::ItemFlags SearchBackend::flags(const QModelIndex &index) const
{
    return Qt::ItemFlag::ItemIsSelectable;
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
}
