#include "editorbackend.h"
#include "utilities.h"
#include "settings.h"
#include "gitmanager.h"

#include <QFile>
#include <QDir>
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

const QSet<QString> EditorBackend::decorators{"**", "__", "~~"};

EditorBackend::EditorBackend(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursorPosition(-1)
    , m_selectionStart(0)
    , m_selectionEnd(0)
{
    connect(this, &EditorBackend::fileSaved, GitManager::getInstance(), &GitManager::slotFileSaved);
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
        m_markdownHighlighter.setDocument(document->textDocument());
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

std::optional<QString> EditorBackend::currentWord() const
{
    QTextCursor cursor = textCursor();

    if (cursor.isNull())
        return std::nullopt;

    // TODO check how this works when a partial word or multiple words are selected
    cursor.select(QTextCursor::SelectionType::WordUnderCursor);
    if (!cursor.hasSelection())
        return std::nullopt;

    const QString word = cursor.selectedText();
    cursor.clearSelection();
    return word;
}


bool EditorBackend::hasDecoration(const QString& decorationStart, const QString& decorationEnd) const
{
    QTextCursor cursor = textCursor();

    if (cursor.isNull())
        return false;

    bool startsWithDecorator = false;
    bool endsWithDecorator = false;

    if (cursor.hasSelection())
    {
        const int originalSelectionStart = cursor.selectionStart();
        const int originalSelectionEnd = cursor.selectionEnd();
        cursor.setPosition(originalSelectionStart);
        cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor, decorationStart.length());
        startsWithDecorator = cursor.selectedText().startsWith(decorationStart);

        cursor.setPosition(originalSelectionEnd);
        cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, decorationEnd.length());
        endsWithDecorator = cursor.selectedText().startsWith(decorationEnd);

        cursor.setPosition(originalSelectionStart);
        cursor.setPosition(originalSelectionEnd, QTextCursor::MoveMode::KeepAnchor);
    }
    else
    {
        const int originalPosition = cursorPosition();
        cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
        cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor, decorationStart.length());
        qDebug() << "EditorBackend::hasDecoration -> selectedTextStart: " << cursor.selectedText();
        startsWithDecorator = cursor.selectedText().startsWith(decorationStart);

        cursor.setPosition(originalPosition);
        cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
        cursor.clearSelection();
        cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, decorationEnd.length());
        qDebug() << "EditorBackend::hasDecoration -> selectedTextEnd: " << cursor.selectedText();
        endsWithDecorator = cursor.selectedText().startsWith(decorationEnd);
        cursor.setPosition(originalPosition);
    }

    return startsWithDecorator && endsWithDecorator;
}

bool EditorBackend::decorateCurrentWord(const QString& decorationStart, const QString& decorationEnd)
{
    QTextCursor cursor = textCursor();

    if (cursor.isNull())
        return false;

    const int pos = cursor.position();
    cursor.beginEditBlock();
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    cursor.insertText(decorationStart);
    cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    cursor.insertText(decorationEnd);
    cursor.setPosition(pos + decorationStart.length());
    cursor.endEditBlock();
    return true;
}

bool EditorBackend::removeDecorationFromCurrentWord(const QString& decorationStart, const QString& decorationEnd)
{
    QTextCursor cursor = textCursor();

    if (cursor.isNull() || !hasDecoration(decorationStart, decorationEnd))
        return false;

    const int originalPosition = cursor.position();

//    cursor.select(QTextCursor::SelectionType::WordUnderCursor);
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> current Word"
//             << cursor.selectedText();

    cursor.beginEditBlock();
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> originalPosition = "
             << cursor.position();
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> StartOfWord = "
             << cursor.position();
    deletePreviousNChars(&cursor, decorationStart.length());
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> currentPosition = "
             << cursor.position();
    cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> EndOfWord = "
             << cursor.position();
    deleteNChars(&cursor, decorationEnd.length());
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> currentPosition = "
             << cursor.position();
    cursor.setPosition(originalPosition - decorationStart.length());
    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> shouldBeOriginalPosition - 2 = "
             << cursor.position();
    cursor.endEditBlock();
    return true;
}

bool EditorBackend::decorateCurrentSelection(const QString& decorationStart, const QString& decorationEnd)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull() || !cursor.hasSelection())
        return false;

    const int originalSelectionStart = cursor.selectionStart();
    const int originalSelectionEnd = cursor.selectionEnd();
    cursor.beginEditBlock();
    cursor.setPosition(selectionStart());
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    cursor.insertText(decorationStart);
    cursor.setPosition(originalSelectionEnd + decorationStart.length());
    cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    cursor.insertText(decorationEnd);
    cursor.setPosition(originalSelectionStart + decorationStart.length());
    cursor.clearSelection();
    cursor.setPosition(originalSelectionEnd + decorationStart.length(), QTextCursor::MoveMode::KeepAnchor);
    cursor.endEditBlock();
    return true;
}

bool EditorBackend::removeDecorationFromCurrentSelection(const QString& decorationStart, const QString& decorationEnd)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull() || !cursor.hasSelection() || !hasDecoration(decorationStart, decorationEnd))
        return false;

    const int originalSelectionStart = cursor.selectionStart();
    const int originalSelectionEnd = cursor.selectionEnd();
    cursor.beginEditBlock();
    cursor.setPosition(selectionStart());
    cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    deletePreviousNChars(&cursor, decorationStart.length());
    cursor.setPosition(originalSelectionEnd - decorationStart.length());
    cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    deleteNChars(&cursor, decorationEnd.length());
    cursor.setPosition(originalSelectionStart - decorationStart.length());
    cursor.clearSelection();
    cursor.setPosition(originalSelectionEnd - decorationStart.length(), QTextCursor::MoveMode::KeepAnchor);
    cursor.endEditBlock();
    return true;
}

void EditorBackend::toggleDecoration(const QString& decorationStart, const QString& decorationEnd)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;

    if (cursor.hasSelection())
    {
        if (hasDecoration(decorationStart, decorationEnd))
        {
            removeDecorationFromCurrentSelection(decorationStart, decorationEnd);
        }
        else
        {
            decorateCurrentSelection(decorationStart, decorationEnd);
        }
    }
    else if (auto word = currentWord())
    {
        if (hasDecoration(decorationStart, decorationEnd))
        {
            removeDecorationFromCurrentWord(decorationStart, decorationEnd);
        }
        else
        {
            decorateCurrentWord(decorationStart, decorationEnd);
        }
    }
}

bool EditorBackend::bold() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;

    return hasDecoration("**", "**");
}

void EditorBackend::setBold(bool bold)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    if (bold == EditorBackend::bold())
        return;
    toggleDecoration("**", "**");
    emit boldChanged();
}

bool EditorBackend::italic() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;

    return hasDecoration("_", "_");
}

void EditorBackend::setItalic(bool italic)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    if (italic == EditorBackend::italic())
        return;
    toggleDecoration("_", "_");
    emit italicChanged();
}

bool EditorBackend::inlineCode() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;

    return hasDecoration("`", "`");
}

void EditorBackend::setInlineCode(bool inlineCode)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    if (inlineCode == EditorBackend::inlineCode())
        return;
    toggleDecoration("`", "`");
    emit inlineCodeChanged();
}

bool EditorBackend::math() const
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return false;

    return hasDecoration("$", "$");
}

void EditorBackend::setMath(bool math)
{
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
    if (math == EditorBackend::math())
        return;
    toggleDecoration("$", "$");
    emit inlineCodeChanged();
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
        return QStringLiteral("untitled.md");
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

QString EditorBackend::documentTitle() const
{
    if (!m_documentTitle.isEmpty())
    {
        return m_documentTitle;
    }
    return QString("Untitled");
}

void EditorBackend::loadAbsolutePath(const QString &absolutePath)
{
    loadUrl(QUrl::fromLocalFile(absolutePath));
}

void EditorBackend::loadRelativePath(const QString &relativePath)
{
    loadUrl(QUrl::fromLocalFile(Settings::getInstance()->rootDirectory() + QDir::separator() + relativePath));
}

void EditorBackend::addLinkTemplate()
{
    toggleDecoration("[", "]()");
    QTextCursor cursor = textCursor();
    if (cursor.isNull())
        return;
}

void EditorBackend::loadUrl(const QUrl &fileUrl)
{
    if (fileUrl == m_fileUrl)
        return;

    Q_ASSERT(fileUrl.isLocalFile());
    const auto fileName = fileUrl.toLocalFile();

    if (QFile::exists(fileName)) {
        QFile file(fileName);
        if (file.open(QFile::ReadOnly)) {
            QByteArray data = file.readAll();
            QTextCodec *codec = QTextCodec::codecForHtml(data);
            if (QTextDocument *doc = textDocument())
                doc->setModified(false);

            auto text = codec->toUnicode(data);

            emit loaded(text);

            auto titleOpt = Utilities::parseTitle(text);
            if (titleOpt) {
                m_documentTitle = *titleOpt;
            }
            else {
                m_documentTitle = QFileInfo(file).baseName();
            }
            emit documentTitleChanged();
            reset();
        }
    }
    else {
        qWarning() << "EditorBackend::loadUrl -> file " << fileUrl.toLocalFile() << "does not exist";
    }

    m_fileUrl = fileUrl;
    emit fileUrlChanged();
}

void EditorBackend::saveAs(const QUrl &fileUrl)
{
    QTextDocument *doc = textDocument();
    if (!doc)
        return;

    auto filePathOpt = Utilities::ensureLocalFile(fileUrl);
    if (!filePathOpt)
        return;
    const QString filePath = *filePathOpt;
    const bool isHtml = QFileInfo(filePath).suffix().contains(QLatin1String("htm"));
    QFile file(filePath);
    if (!file.open(QFile::WriteOnly | QFile::Truncate | (isHtml ? QFile::NotOpen : QFile::Text))) {
        emit error(tr("Cannot save: ") + file.errorString());
        return;
    }
    file.write((isHtml ? doc->toHtml() : doc->toPlainText()).toUtf8());
    file.close();
    emit fileSaved(file.fileName());
    setModified(false);

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
    emit inlineCodeChanged();
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

void EditorBackend::deleteNChars(QTextCursor* cursor, int n)
{
    qDebug() << "EditorBackend::deleteNChars -> cursor.position() = "
             << cursor->position();

    cursor->clearSelection();
    qDebug() << "EditorBackend::deleteNChars -> cursor.position() = "
             << cursor->position();
    cursor->setPosition(cursor->position() + n, QTextCursor::MoveMode::KeepAnchor);
    qDebug() << "EditorBackend::deleteNChars " << cursor->selectedText();
    cursor->removeSelectedText();
}

void EditorBackend::deletePreviousNChars(QTextCursor* cursor, int n)
{
    qDebug() << "EditorBackend::deletePreviousNChars -> cursor.position() = "
             << cursor->position();

    cursor->clearSelection();
    qDebug() << "EditorBackend::deletePreviousNChars -> cursor.position() = "
             << cursor->position();
    cursor->setPosition(cursor->position() - n, QTextCursor::MoveMode::KeepAnchor);
    qDebug() << "EditorBackend::deletePreviousNChars " << cursor->selectedText();
    cursor->removeSelectedText();
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

