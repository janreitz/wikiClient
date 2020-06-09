#include "utilities.h"

#include <QFile>
#include <QDir>
#include <QUrl>
#include <QTextStream>
#include <QDebug>
#include <QRegularExpression>

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
            QRegularExpression headerRegEx("#\\s(?<title>.*)");
            auto match = headerRegEx.match(fileContent);
            if (match.hasMatch())
            {
                return match.captured("title");
            }
        }
        return std::nullopt;
    }

    std::optional<QString> ensureLocalPathAndExists(const QString &filePathOrUrl)
    {
        QString localPath;

        if (filePathOrUrl.startsWith("file:"))
        {
            localPath = QUrl(filePathOrUrl).toLocalFile();
        }
        else
        {
            localPath = filePathOrUrl;
        }
        if (QFile::exists(localPath) || QDir(localPath).exists())
        {
            return localPath;
        }
        else
        {
            return std::nullopt;
        }
    }

}
