#include "linksuggestionprovider.h"

LinkSuggestionProvider::LinkSuggestionProvider()
    : AbstractSuggestionProvider(nullptr)
{

}

QList<Suggestion> LinkSuggestionProvider::getSuggestion(const QString& searchString) const
{
    Q_UNUSED(searchString);
    auto q_opt = mDBManager->getQuery();
    if (!q_opt)
    {
        qDebug() << "SuggestionProviderLink::getSuggestion -> could not get a query";
        return QList<Suggestion>();
    }
    QSqlQuery q = q_opt.value();
    return QList<Suggestion>();
}
