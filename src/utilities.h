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

    float vectorLength(const QPointF& vec);
    std::optional<QPointF> normalizeVector(const QPointF& vec);
    QPointF normalizeVectorOrRandomize(const QPointF& vec);
    QPointF randomNormalVector();
    QPointF elementwiseMultiplication(const QPointF& vec1, const QPointF& vec2);
    float scalarProduct(const QPointF& vec1, const QPointF& vec2);
    QPointF vectorProjection(const QPointF& projectThis, const QPointF& ontoThis);

//    Gets all surrounding cells
//    X X X
//    X   X
//    X X X
    QList<QPoint> getNeighboringCells(const QPoint& cell);



}

uint qHash(const QPointF& point);

#endif // UTILITIES_H
