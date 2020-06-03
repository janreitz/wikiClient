#ifndef EDITORBACKEND_H
#define EDITORBACKEND_H

#include <QObject>
#include <QTextDocument>

class EditorBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
public:

    Q_INVOKABLE QString readFile(const QString& filePath);

    explicit EditorBackend(QObject *parent = nullptr);

    void setText(const QString& text) {
        if (text != mText) {
            mText = text;
            emit textChanged();
        }
    };
    QString text() const {return mText;}

public slots:

signals:
    void textChanged();
    void signalFileOpened(const QString& content);

private:

    QString mText;
};

#endif // EDITORBACKEND_H
