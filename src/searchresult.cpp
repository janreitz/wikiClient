#include "searchresult.h"

SearchResult::SearchResult(const QString& name, const QString& title, const QString& matchContext)
    : QObject(nullptr)
    , m_name(name)
    , m_title(title)
    , m_matchContext(matchContext)
{

}

SearchResult::SearchResult(const SearchResult &other)
{
    m_name = other.name();
    m_title = other.title();
    m_matchContext = other.matchContext();
}

QString SearchResult::name() const
{
    return m_name;
}

QString SearchResult::title() const
{
    return m_title;
}

QString SearchResult::matchContext() const
{
    return m_matchContext;
}
