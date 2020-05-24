#include "abstractsuggestionprovider.h"

AbstractSuggestionProvider::AbstractSuggestionProvider(DBManager* dbManager)
    : QObject(nullptr)
    , mDBManager(dbManager)
{

}
