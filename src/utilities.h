#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <optional>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath);
    std::optional<QString> parseTitle(const QString& fileContent);
}

#endif // UTILITIES_H
