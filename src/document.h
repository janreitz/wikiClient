#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QString>
#include <QList>
#include <optional>

class Document: public QObject
{
    Q_OBJECT

public:

    enum class LinkType {
        INTERNAL,
        EXTERNAL,
        MEDIA
    };
    Q_ENUM(LinkType);

    struct Link
    {
        QString source;
        QString target;
        QString display;
        LinkType linkType;

        Link(const QString& source, const QString& target, const QString& display);
    };

    Document();
    Document(const Document& other);

    QString name;
    QString title;
    QString content;
    QList<Link> links;

    static std::optional<Document> fromString(const QString& docName, const QString& docString);
};

#endif // DOCUMENT_H
