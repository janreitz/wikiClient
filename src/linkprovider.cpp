#include "linkprovider.h"

LinkProvider::LinkProvider()
{
}

QString LinkProvider::documentTitle()
{
    return m_documentTitle;
}

QStringList LinkProvider::links()
{
    return m_links;
}

QStringList LinkProvider::backLinks()
{
    return m_backLinks;
}

void LinkProvider::setDocumentTitle(const QString &docTitle)
{
    m_documentTitle = docTitle;
    m_links = getLinks(m_documentTitle);
    m_backLinks = getBackLinks(m_documentTitle);

    emit documentTitleChanged();
    emit linksChanged();
    emit backLinksChanged();
}

QStringList LinkProvider::getLinks(const QString& docTitle)
{
    if (!m_dbOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;

    m_query.exec("SELECT target From links WHERE source = (SELECT name From documents WHERE title = '" + docTitle + "');");
    while (m_query.next())
    {
        links << m_query.value(0).toString();
    }
    return links;
}

QStringList LinkProvider::getBackLinks(const QString& docTitle)
{
    if (!m_dbOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;
    m_query.exec("SELECT source From links WHERE target = (SELECT name FROM documents WHERE title = '" + docTitle + "');");
    while (m_query.next())
    {
        links << m_query.value(0).toString();
    }
    return links;
}
