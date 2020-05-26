#include "utilities.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& originalFilePath)
    {
        auto filePath = originalFilePath;

        if (filePath.contains("file:///"))
        {
            filePath.remove(0,8);
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
}
