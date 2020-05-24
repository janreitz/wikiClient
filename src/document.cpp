#include "document.h"

#include <QRegularExpression>

Link::Link(const QString& source, const QString& target, const QString& display)
    : source(source)
    , target(target)
    , display(display)
{}

std::optional<Document> Document::fromString(const QString& docName, const QString& docString)
{
    if (docString.isEmpty()) {return std::nullopt;}

    Document doc;
    doc.name = docName;
    doc.content = docString;

    if (docString.at(0) == "#")
    {
        QRegularExpression headerRegEx("#\\s(?<title>.*)");
        auto match = headerRegEx.match(docString);
        if (match.hasMatch())
        {
            doc.title = match.captured("title");
        }
        else
        {
            doc.title = docName;
        }
    }
    else
    {
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
