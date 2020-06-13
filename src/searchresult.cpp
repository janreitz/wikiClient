#include "searchresult.h"

SearchResult::SearchResult(const QString& title, const QString& matchContext)
    : QObject(nullptr)
    , m_title(title)
    , m_matchContext(matchContext)
{

}

SearchResult::SearchResult(const SearchResult &other)
{
    m_title = other.title();
    m_matchContext = other.matchContext();
}

QString SearchResult::title() const
{
    return m_title;
}

QString SearchResult::matchContext() const
{
    return m_matchContext;
}
