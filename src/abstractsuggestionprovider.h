#ifndef ABSTRACTSUGGESTIONPROVIDER_H
#define ABSTRACTSUGGESTIONPROVIDER_H

#include <QObject>
#include <QSqlQueryModel>

#include "dbmanager.h"
#include "suggestion.h"

class AbstractSuggestionProvider : public QObject
{
    Q_OBJECT
public:
    explicit AbstractSuggestionProvider(DBManager* dbManager);

    //Q_INVOKABLE virtual QList<Suggestion> getSuggestion(const QString& searchString) const = 0;

signals:

protected:
    DBManager* mDBManager;
    QSqlQuery mQuery;
    bool mDBOpen;
};

#endif // ABSTRACTSUGGESTIONPROVIDER_H
