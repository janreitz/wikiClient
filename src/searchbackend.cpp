#include "searchbackend.h"

SearchBackend::SearchBackend(DBManager* dbManager)
    : m_dbManager(dbManager)
{

}

int SearchBackend::rowCount(const QModelIndex &) const
{
    return m_searchResults.count();
}

QVariant SearchBackend::data(const QModelIndex &index, int role) const
{
    if (index.row() < rowCount())
        switch (role) {
        case FullNameRole: return m_contacts.at(index.row()).fullName;
        case AddressRole: return m_contacts.at(index.row()).address;
        case CityRole: return m_contacts.at(index.row()).city;
        case NumberRole: return m_contacts.at(index.row()).number;
        default: return QVariant();
    }
    return QVariant();
}
