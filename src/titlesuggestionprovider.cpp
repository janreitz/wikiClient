#include "titlesuggestionprovider.h"

#include <QtSql/QSqlRecord>

TitleSuggestionProvider::TitleSuggestionProvider(DBManager* dBManager)
    : AbstractSuggestionProvider(dBManager)
{
}

QStringList TitleSuggestionProvider::getSuggestion(const QString& searchString)
{
    if (!mDBOpen) {
        qWarning() << "TitleSuggestionProvider::getSuggestion -> DB not open";
        return QStringList();
    }

    QStringList titleSuggestions;
    mQuery.exec("SELECT Title FROM Documents WHERE Title LIKE '%" + searchString + "%'");
    while (mQuery.next())
    {
        titleSuggestions << mQuery.value(0).toString();
    }
    return titleSuggestions;
}
