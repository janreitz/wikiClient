#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QString>
#include <QList>
#include <optional>

struct Link
{
    QString source;
    QString target;
    QString display;

    Link(const QString& source, const QString& target, const QString& display);
};

struct Document
{
    QString name;
    QString title;
    QString content;
    QList<Link> links;

    static std::optional<Document> fromString(const QString& docName, const QString& docString);
};

#endif // DOCUMENT_H
