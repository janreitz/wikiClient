#include "linkprovider.h"

LinkProvider::LinkProvider(DBManager* dBManager)
    : AbstractSuggestionProvider(dBManager)
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
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;

    mQuery.exec("SELECT Target From Links Where Source = (Select name From Documents Where Title = '" + docTitle + "');");
    while (mQuery.next())
    {
        links << mQuery.value(0).toString();
    }
    return links;
}

QStringList LinkProvider::getBackLinks(const QString& docTitle)
{
    if (!mDBOpen) {
        qWarning() << "LinkProvider::getBackLinks -> DB not open";
        return QStringList();
    }

    QStringList links;
    mQuery.exec("SELECT Source From Links Where Target = (Select name From Documents Where Title = '" + docTitle + "');");
    while (mQuery.next())
    {
        links << mQuery.value(0).toString();
    }
    return links;
}
