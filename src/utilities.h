#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <optional>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath);
    std::optional<QString> parseTitle(const QString& fileContent);
    // Convert to local path if it is a url and check for existance.
    std::optional<QString> ensureLocalPathAndExists(const QString& filePathOrUrl);
}

#endif // UTILITIES_H
