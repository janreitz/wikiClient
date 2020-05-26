#include "editorbackend.h"
#include "utilities.h"

#include <QDebug>

EditorBackend::EditorBackend(QObject *parent) : QObject(parent)
{

}

QString EditorBackend::readFile(const QString& filePath)
{
    if (auto content = Utilities::stringFromFile(filePath); content)
    {
        return *content;
    }
    return QString();
}
