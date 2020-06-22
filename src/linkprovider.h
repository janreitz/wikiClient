#ifndef LINKPROVIDER_H
#define LINKPROVIDER_H

#include <QStringList>

#include "abstractdbclient.h"

class LinkProvider: public AbstractDBClient
{
    Q_OBJECT
    Q_PROPERTY(QString documentTitle READ documentTitle WRITE setDocumentTitle NOTIFY documentTitleChanged)
    Q_PROPERTY(QStringList links READ links NOTIFY linksChanged)
    Q_PROPERTY(QStringList backLinks READ backLinks NOTIFY backLinksChanged)

public:
    explicit LinkProvider();

    Q_INVOKABLE QStringList getLinks(const QString& docTitle);
    Q_INVOKABLE QStringList getBackLinks(const QString& docTitle);

    QString documentTitle();
    void setDocumentTitle(const QString& docTitle);

    QStringList links();

    QStringList backLinks();

signals:
    void documentTitleChanged();
    void linksChanged();
    void backLinksChanged();

private:

    QString m_documentTitle;
    QStringList m_links;
    QStringList m_backLinks;

};

#endif // LINKPROVIDER_H
