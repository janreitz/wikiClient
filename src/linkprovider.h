#ifndef LINKPROVIDER_H
#define LINKPROVIDER_H

#include <QStringList>

#include "abstractsuggestionprovider.h"

class LinkProvider: public AbstractSuggestionProvider
{
    Q_OBJECT
public:
    explicit LinkProvider(DBManager* dBManager);

    Q_INVOKABLE QStringList getLinks(const QString& docTitle);
    Q_INVOKABLE QStringList getBackLinks(const QString& docTitle);
};

#endif // LINKPROVIDER_H
