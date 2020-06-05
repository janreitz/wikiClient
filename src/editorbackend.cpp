#include "editorbackend.h"

#include <QFile>
#include <QFileInfo>
#include <QFileSelector>
#include <QQmlFile>
#include <QQmlFileSelector>
#include <QQuickTextDocument>
#include <QTextCharFormat>
#include <QTextCodec>
#include <QTextCursor>
#include <QTextDocument>
#include <QDebug>

EditorBackend::EditorBackend(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
{
}

QQuickTextDocument *EditorBackend::document() const
{
    return m_document;
}

void EditorBackend::setDocument(QQuickTextDocument *document)
{
    if (document == m_document)
        return;

    if (m_document)
        m_document->textDocument()->disconnect(this);
    m_document = document;
    if (m_document){
        connect(m_document->textDocument(), &QTextDocument::modificationChanged, this, &EditorBackend::modifiedChanged);
        mMarkdownHighlighter.setDocument(document->textDocument());
    }
    emit documentChanged();
}

int EditorBackend::cursorPosition() const
{
    return m_cursorPosition;
}

void EditorBackend::setCursorPosition(int position)
{
    if (position == m_cursorPosition)
        return;

    m_cursorPosition = position;
    reset();
    emit cursorPositionChanged();
}

int EditorBackend::selectionStart() const
{
    return m_selectionStart;
}

void EditorBackend::setSelectionStart(int position)
{
    if (position == m_selectionStart)
        return;

    m_selectionStart = position;
    emit selectionStartChanged();
}

int EditorBackend::selectionEnd() const
{
    return m_selectionEnd;
}

void EditorBackend::setSelectionEnd(int position)
{
    if (position == m_selectionEnd)
        return;

    m_selectionEnd = position;
    emit selectionEndChanged();
}

QString EditorBackend::fontFamily() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QString();
    QTextCharFormat format = cursor.charFormat();
    return format.font().family();
}

void EditorBackend::setFontFamily(const QString &family)
{
    QTextCharFormat format;
    format.setFontFamily(family);
    mergeFormatOnWordOrSelection(format);
    emit fontFamilyChanged();
}

QColor EditorBackend::textColor() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = cursor.charFormat();
    return format.foreground().color();
}

void EditorBackend::setTextColor(const QColor &color)
{
    QTextCharFormat format;
    format.setForeground(QBrush(color));
    mergeFormatOnWordOrSelection(format);
    emit textColorChanged();
}

Qt::Alignment EditorBackend::alignment() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return Qt::AlignLeft;
    return textCursor().blockFormat().alignment();
}

void EditorBackend::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    QTextCursor cursor = textCursor();
    cursor.mergeBlockFormat(format);
    emit alignmentChanged();
}

bool EditorBackend::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontWeight() == QFont::Bold;
}

void EditorBackend::setBold(bool bold)
{
    QTextCharFormat format;
    format.setFontWeight(bold ? QFont::Bold : QFont::Normal);
    mergeFormatOnWordOrSelection(format);
    emit boldChanged();
}

bool EditorBackend::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontItalic();
}

void EditorBackend::setItalic(bool italic)
{
    QTextCharFormat format;
    format.setFontItalic(italic);
    mergeFormatOnWordOrSelection(format);
    emit italicChanged();
}

bool EditorBackend::underline() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;
    return textCursor().charFormat().fontUnderline();
}

void EditorBackend::setUnderline(bool underline)
{
    QTextCharFormat format;
    format.setFontUnderline(underline);
    mergeFormatOnWordOrSelection(format);
    emit underlineChanged();
}

int EditorBackend::fontSize() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return 0;
    QTextCharFormat format = cursor.charFormat();
    return format.font().pointSize();
}

void EditorBackend::setFontSize(int size)
{
    if (size <= 0)
        return;

    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);

    if (cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
        return;

    QTextCharFormat format;
    format.setFontPointSize(size);
    mergeFormatOnWordOrSelection(format);
    emit fontSizeChanged();
}

QString EditorBackend::fileName() const
{
    const QString filePath = QQmlFile::urlToLocalFileOrQrc(m_fileUrl);
    const QString fileName = QFileInfo(filePath).fileName();
    if (fileName.isEmpty())
        return QStringLiteral("untitled.txt");
    return fileName;
}

QString EditorBackend::fileType() const
{
    return QFileInfo(fileName()).suffix();
}

QUrl EditorBackend::fileUrl() const
{
    return m_fileUrl;
}

void EditorBackend::load(const QUrl &fileUrl)
{
    if (fileUrl == m_fileUrl)
        return;

    QQmlEngine *engine = qmlEngine(this);
    if (!engine) {
        qWarning() << "load() called before EditorBackend has QQmlEngine";
        return;
    }

    const QUrl path = QQmlFileSelector::get(engine)->selector()->select(fileUrl);
    const QString fileName = QQmlFile::urlToLocalFileOrQrc(path);
    if (QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            QTextCodec *codec = QTextCodec::codecForHtml(data);
            if (QTextDocument *doc = textDocument())
                doc->setModified(false);

            emit loaded(codec->toUnicode(data));
            reset();
        }
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

void EditorBackend::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    const QString filePath = fileUrl.toLocalFile();
    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();

    if (fileUrl == m_fileUrl)
        return;

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

bool EditorBackend::currentFileUrlExists() const
{
    return QFile::exists(m_fileUrl.toLocalFile());
}

void EditorBackend::reset()
{
    emit fontFamilyChanged();
    emit alignmentChanged();
    emit boldChanged();
    emit italicChanged();
    emit underlineChanged();
    emit fontSizeChanged();
    emit textColorChanged();
}

QTextCursor EditorBackend::textCursor() const
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return QTextCursor();

    QTextCursor cursor = QTextCursor(doc);
    if (m_selectionStart != m_selectionEnd) {
        cursor.setPosition(m_selectionStart);
        cursor.setPosition(m_selectionEnd, QTextCursor::KeepAnchor);
    } else {
        cursor.setPosition(m_cursorPosition);
    }
    return cursor;
}

QTextDocument *EditorBackend::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}

void EditorBackend::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    QTextCursor cursor = textCursor();
    if (!cursor.hasSelection())
        cursor.select(QTextCursor::WordUnderCursor);
    cursor.mergeCharFormat(format);
}

bool EditorBackend::modified() const
{
    return m_document && m_document->textDocument()->isModified();
}

void EditorBackend::setModified(bool m)
{
    if (m_document) {
        m_document->textDocument()->setModified(m);
        emit modifiedChanged();
    }
}

