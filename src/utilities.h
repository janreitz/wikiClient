#ifndef UTILITIES_H
#define UTILITIES_H

#include <QString>
#include <optional>

namespace Utilities
{
    std::optional<QString> stringFromFile(const QString& filePath);
}

#endif // UTILITIES_H
