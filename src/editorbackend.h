#ifndef EDITORBACKEND_H
#define EDITORBACKEND_H

#include <QObject>

class EditorBackend : public QObject
{
    Q_OBJECT
public:

    explicit EditorBackend(QObject *parent = nullptr);

    Q_INVOKABLE QString readFile(const QString& filePath);

public slots:

signals:
    void signalFileOpened(const QString& content);
};

#endif // EDITORBACKEND_H
