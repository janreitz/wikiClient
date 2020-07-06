#include "document.h"
#include "utilities.h"

#include <QRegularExpression>

Document::Link::Link(const QString& source, const QString& _target, const QString& display)
    : source(source)
    , display(display)
{
    // is it am external link?
    // some type of http*
    // internal links don't have '.' in them
    if (_target.contains("http") && _target.contains("."))
    {
        linkType = LinkType::EXTERNAL;
        target = _target;
    }
    // is it a media link?
    // some type of file extension *.*
    else if (_target.contains("."))
    {
        linkType = LinkType::MEDIA;
        // Remove leading '/' if present.
        // In gollum this indicates top level directory
        if (target.startsWith("/"))
            target.remove(0,1);
        target = _target;
    }
    // Should be an internal Link
    else
    {
        linkType = LinkType::INTERNAL;
        // Remove leading '/' if present.
        // In gollum this indicates top level directory
        if (_target.startsWith("/"))
            target.remove(0,1);
        target = _target;
    }
}

Document::Document()
    : QObject(nullptr)
{
}

Document::Document(const Document &other)
{
    name = other.name;
    title = other.title;
    content = other.content;
    links = other.links;
}

std::optional<Document> Document::fromString(const QString& docName, const QString& docString)
{
    if (docString.isEmpty()) {return std::nullopt;}

    Document doc;
    doc.name = docName;
    doc.content = docString;

    auto titleOpt = Utilities::parseTitle(docString);
    if (titleOpt) {
        doc.title = *titleOpt;
    }
    else {
        doc.title = docName;
    }

    QRegularExpression linkRegEx("\\[(?<display>.*?)\\]\\((?<target>.*?)\\)");

    auto matchIter = linkRegEx.globalMatch(docString);
    while (matchIter.hasNext())
    {
        QRegularExpressionMatch match = matchIter.next();
        doc.links << Link(docName, match.captured("target"), match.captured("display"));
    }

    return doc;
}
