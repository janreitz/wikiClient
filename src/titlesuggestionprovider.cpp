#include "titlesuggestionprovider.h"

#include <QtSql/QSqlRecord>

TitleSuggestionProvider::TitleSuggestionProvider()
{
}

QStringList TitleSuggestionProvider::getSuggestion(const QString& searchString)
{
    if (!m_dbOpen) {
        qWarning() << "TitleSuggestionProvider::getSuggestion -> DB not open";
        return QStringList();
    }

    QStringList titleSuggestions;
    m_query.exec("SELECT title FROM documents WHERE title LIKE '%" + searchString + "%'");
    while (m_query.next())
    {
        titleSuggestions << m_query.value(0).toString();
    }
    return titleSuggestions;
}
