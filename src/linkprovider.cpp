#include "linkprovider.h"

LinkProvider::LinkProvider(DBManager* dBManager)
    : AbstractSuggestionProvider(dBManager)
{
}

QStringList LinkProvider::getLinks(const QString& docTitle)
{
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;
    mQuery.exec("SELECT Target FROM Links WHERE Source = '" + docTitle + "';");
    while (mQuery.next())
    {
        links << mQuery.value(0).toString();
    }
    return links;
}

QStringList LinkProvider::getBackLinks(const QString& docTitle)
{
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;
    mQuery.exec("SELECT Source FROM Links WHERE Target = '" + docTitle + "';");
    while (mQuery.next())
    {
        links << mQuery.value(0).toString();
    }
    return links;
}
