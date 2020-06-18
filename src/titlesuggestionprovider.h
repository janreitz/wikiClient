#ifndef TITLESUGGESTIONPROVIDER_H
#define TITLESUGGESTIONPROVIDER_H
#include "abstractdbclient.h"
#include <QtSql/QSqlQueryModel>

class TitleSuggestionProvider: public AbstractDBClient
{
    Q_OBJECT

public:
    explicit TitleSuggestionProvider(DBManager* dBManager);

    Q_INVOKABLE QStringList getSuggestion(const QString& searchString);

private:

};

#endif // TITLESUGGESTIONPROVIDER_H
