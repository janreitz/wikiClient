#include "utilities.h"

#include <QFile>
#include <QTextStream>
#include <QDebug>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath)
    {
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
