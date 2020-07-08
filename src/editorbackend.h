#ifndef EDITORBACKEND_H
#define EDITORBACKEND_H

#include <QObject>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <optional>

#include "markdownhighlighter.h"

class EditorBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QQuickTextDocument *document READ document WRITE setDocument NOTIFY documentChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart WRITE setSelectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd WRITE setSelectionEnd NOTIFY selectionEndChanged)

    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QString fontFamily READ fontFamily WRITE setFontFamily NOTIFY fontFamilyChanged)
    Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment NOTIFY alignmentChanged)

    Q_PROPERTY(bool bold READ bold WRITE setBold NOTIFY boldChanged)
    Q_PROPERTY(bool italic READ italic WRITE setItalic NOTIFY italicChanged)
    Q_PROPERTY(bool inlineCode READ inlineCode WRITE setInlineCode NOTIFY inlineCodeChanged)
    Q_PROPERTY(bool math READ math WRITE setMath NOTIFY mathChanged)

    Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize NOTIFY fontSizeChanged)

    Q_PROPERTY(QString fileName READ fileName NOTIFY fileUrlChanged)
    Q_PROPERTY(QString fileType READ fileType NOTIFY fileUrlChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl NOTIFY fileUrlChanged)
    Q_PROPERTY(QString documentTitle READ documentTitle NOTIFY documentTitleChanged)

    Q_PROPERTY(bool modified READ modified NOTIFY modifiedChanged)

public:

    Q_INVOKABLE void addLinkTemplate();

    Q_INVOKABLE void loadUrl(const QUrl &fileUrl);
    Q_INVOKABLE void loadAbsolutePath(const QString &filePath);
    // Relative to current Working Directory
    Q_INVOKABLE void loadRelativePath(const QString &filePath);
    Q_INVOKABLE void saveAs(const QUrl &fileUrl);
    Q_INVOKABLE bool currentFileUrlExists() const;

    explicit EditorBackend(QObject *parent = nullptr);

    QQuickTextDocument *document() const;
    void setDocument(QQuickTextDocument *document);

    int cursorPosition() const;
    void setCursorPosition(int position);

    int selectionStart() const;
    void setSelectionStart(int position);

    int selectionEnd() const;
    void setSelectionEnd(int position);

    QString fontFamily() const;
    void setFontFamily(const QString &family);

    QColor textColor() const;
    void setTextColor(const QColor &color);

    Qt::Alignment alignment() const;
    void setAlignment(Qt::Alignment alignment);

    bool bold() const;
    void setBold(bool bold);

    bool italic() const;
    void setItalic(bool italic);

    bool inlineCode() const;
    void setInlineCode(bool inlineCode);

    bool math() const;
    void setMath(bool math);

    int fontSize() const;
    void setFontSize(int size);

    QString fileName() const;
    QString fileType() const;
    QUrl fileUrl() const;
    QString documentTitle() const;

    bool modified() const;
    void setModified(bool m);

public slots:

signals:
    void documentChanged();
    void cursorPositionChanged();
    void selectionStartChanged();
    void selectionEndChanged();

    void fontFamilyChanged();
    void textColorChanged();
    void alignmentChanged();

    void boldChanged();
    void italicChanged();
    void inlineCodeChanged();
    void mathChanged();

    void fontSizeChanged();

    void textChanged();
    void fileUrlChanged();
    void documentTitleChanged();

    void loaded(const QString &text);
    void error(const QString &message);
    void fileSaved(const QString& fileName);

    void modifiedChanged();

private:

    void reset();
    QTextCursor textCursor() const;
    QTextDocument *textDocument() const;
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

    void deleteNChars(QTextCursor* cursor, int n);
    void deletePreviousNChars(QTextCursor* cursor,int n);

    bool hasDecoration(const QString& decorationStart, const QString& decorationEnd) const;
    bool decorateCurrentWord(const QString& decorationStart, const QString& decorationEnd);
    bool decorateCurrentSelection(const QString& decorationStart, const QString& decorationEnd);
    bool removeDecorationFromCurrentWord(const QString& decorationStart, const QString& decorationEnd);
    bool removeDecorationFromCurrentSelection(const QString& decorationStart, const QString& decorationEnd);
    void toggleDecoration(const QString& decorationStart, const QString& decorationEnd);

    std::optional<QString> currentWord() const;

    QQuickTextDocument *m_document;
    MarkdownHighlighter m_markdownHighlighter;

    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;

    QFont m_font;
    QUrl m_fileUrl;
    QString m_documentTitle;

    static const QSet<QString> decorators;
};

#endif // EDITORBACKEND_H
