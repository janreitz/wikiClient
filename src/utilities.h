#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <QUrl>
#include <optional>
#include <QPointF>
#include <QDir>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath);
    std::optional<QString> parseTitle(const QString& fileContent);
    // Convert to local path if it is a url and check for existance.
    std::optional<QString> ensureLocalFile(const QString &localPath);
    std::optional<QString> ensureLocalFile(const QUrl &localFileUrl);
    std::optional<QString> ensureLocalPathAndExists(const QString& filePathOrUrl);
    std::optional<QString> getRelativePathTo(const QString& dirPath, const QString& filePath);

    double vectorLength(const QPointF& vec);
    std::optional<QPointF> normalizeVector(const QPointF& vec);
    QPointF normalizeVectorOrRandomize(const QPointF& vec);
    QPointF randomNormalVector();
}

#endif // UTILITIES_H
