#include "document.h"
#include "utilities.h"

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
        auto target = match.captured("target");
        if (target.startsWith("/")) {
            target.remove(0,1);
        }
        doc.links << Link(docName, target, match.captured("display"));
    }

    return doc;
}
