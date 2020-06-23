#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <optional>
#include <QPointF>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath);
    std::optional<QString> parseTitle(const QString& fileContent);
    // Convert to local path if it is a url and check for existance.
    std::optional<QString> ensureLocalPathAndExists(const QString& filePathOrUrl);

    double vectorLength(const QPointF& vec);
    std::optional<QPointF> normalizeVector(const QPointF& vec);
    QPointF normalizeVectorOrRandomize(const QPointF& vec);
    QPointF randomNormalVector();
}

#endif // UTILITIES_H
