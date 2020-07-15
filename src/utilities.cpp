#include "utilities.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>
#include <QRandomGenerator>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& originalFilePath)
    {
        auto filePath = originalFilePath;

        if (filePath.startsWith("file:"))
        {
            filePath = QUrl(filePath).toLocalFile();
        }

        auto file = QFile(filePath);
        if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qWarning() << "Utilities::stringFromFile -> Could not open " << filePath;
            return std::nullopt;
        }
        QTextStream in(&file);
        in.setCodec("UTF-8");
        QString content = in.readAll();
        return content;
    }

    std::optional<QString> parseTitle(const QString& fileContent)
    {
        if (fileContent.at(0) == "#")
        {
            QRegularExpression headerRegEx("#\\s(?<title>.*?)(\\\n|\\\r)");
            Q_ASSERT(headerRegEx.isValid());
            auto match = headerRegEx.match(fileContent);
            if (match.hasMatch())
            {
                //qDebug() << match.captured("title");
                return match.captured("title");
            }
        }
        return std::nullopt;
    }

    std::optional<QString> ensureLocalFile(const QString &filePath)
    {
        QUrl fileUrl = QUrl::fromLocalFile(filePath);
        if (fileUrl.isValid())
            return fileUrl.toLocalFile();
        return std::nullopt;
    }

    std::optional<QString> ensureLocalFile(const QUrl &localFileUrl)
    {
        if (localFileUrl.isLocalFile())
            return localFileUrl.toLocalFile();
        return std::nullopt;
    }

    std::optional<QString> ensureLocalPathAndExists(const QString &filePathOrUrl)
    {
        auto localPathOpt = ensureLocalFile(filePathOrUrl);
        if (!localPathOpt)
            return std::nullopt;

        QString localPath = *localPathOpt;

        if (QFile::exists(localPath) || QDir(localPath).exists())
            return localPath;
        else
            return std::nullopt;
    }

    double vectorLength(const QPointF &vec)
    {
        return sqrt(pow(vec.x(), 2) + pow(vec.y(), 2));
    }

    std::optional<QPointF> normalizeVector(const QPointF &vec)
    {
        const double length = vectorLength(vec);
        if (length == 0)
            return std::nullopt;
        return vec/vectorLength(vec);
    }

    QPointF randomNormalVector()
    {
        int randX = QRandomGenerator::global()->bounded(100) - 50;
        const int randY = QRandomGenerator::global()->bounded(100) - 50;
        if (randX == 0 && randY == 0)
        {
            randX = 1;
        }
        return *Utilities::normalizeVector(QPointF(randX, randY));
    }

    QPointF normalizeVectorOrRandomize(const QPointF &vec)
    {
        if (auto normVec = normalizeVector(vec))
            return *normVec;
        return randomNormalVector();
    }

    std::optional<QString> getRelativePathTo(const QString &dirPath, const QString &filePath)
    {
        QDir dir(dirPath);
        if (!dir.exists())
            return std::nullopt;
        if (!dir.makeAbsolute())
            return std::nullopt;
        auto absDirPath = dir.path();
        auto localPathOpt = ensureLocalPathAndExists(filePath);
        if (!localPathOpt)
            return std::nullopt;
        if (localPathOpt->startsWith(absDirPath))
            return localPathOpt->right(localPathOpt->length() - absDirPath.length() - 1); // -1 to remove the leading '/'
        return std::nullopt;
    }

    QPointF elementwiseMultiplication(const QPointF &vec1, const QPointF &vec2)
    {
        return QPointF(vec1.x() * vec2.x(), vec1.y() * vec2.y());
    }

    double scalarProduct(const QPointF &vec1, const QPointF &vec2)
    {
        return vec1.x()*vec2.x() + vec1.y()*vec2.y();
    }

    QPointF vectorProjection(const QPointF &vec1, const QPointF &vec2)
    {
        return (scalarProduct(vec1, vec2) / scalarProduct(vec2, vec2)) * vec2;
    }

}
