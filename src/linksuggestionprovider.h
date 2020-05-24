#ifndef SUGGESTIONPROVIDERLINK_H
#define SUGGESTIONPROVIDERLINK_H

#include <QStringList>

#include "abstractsuggestionprovider.h"

class LinkSuggestionProvider : public AbstractSuggestionProvider
{
public:
    explicit LinkSuggestionProvider();//DBManager* dBManager);

    Q_INVOKABLE QList<Suggestion> getSuggestion(const QString& searchString) const;
};

#endif // SUGGESTIONPROVIDERLINK_H
