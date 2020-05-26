#include "abstractsuggestionprovider.h"

AbstractSuggestionProvider::AbstractSuggestionProvider(DBManager* dbManager)
    : QObject(nullptr)
    , mDBManager(dbManager)
{
    QObject::connect(mDBManager, &DBManager::signalDBOpened, this, [this]{
        mDBOpen = true;
        if (auto q = mDBManager->getQuery(); q)
            mQuery = *q;
    });
    QObject::connect(mDBManager, &DBManager::signalDBClosed, this, [this]{
        mDBOpen = false;
        mQuery.clear();
    });
}
