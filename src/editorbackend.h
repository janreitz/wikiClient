#ifndef EDITORBACKEND_H
#define EDITORBACKEND_H

#include <QObject>
#include <QTextDocument>

class EditorBackend : public QObject
{
    Q_OBJECT
    Q_INVOKABLE QString readFile(const QString& filePath);
    Q_PROPERTY(QString text)

public:

    explicit EditorBackend(QObject *parent = nullptr);

public slots:

signals:
    void signalFileOpened(const QString& content);
};

#endif // EDITORBACKEND_H
