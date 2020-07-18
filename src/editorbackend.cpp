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

EditorBackend::EditorBackend(QObject *parent)
    : QObject(parent)
    , m_document(nullptr)
    , m_cursor()
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
        m_cursor = QTextCursor(m_document->textDocument());
    }
    emit documentChanged();
}

int EditorBackend::cursorPosition() const
{
    return m_cursor.position();
}

void EditorBackend::setCursorPosition(int position)
{
    if (position == cursorPosition())
        return;

    m_cursor.setPosition(position);
    // reset(); this was in the example code, but I don't understand it. Leave for future reference, Chesterton's fence and all that
    emit cursorPositionChanged();
}

int EditorBackend::selectionStart() const
{
    return m_cursor.selectionStart();
}

void EditorBackend::setSelectionStart(int position)
{
    if (position == selectionStart())
        return;

    const int selectionEnd_ = selectionEnd();
    m_cursor.setPosition(position);
    m_cursor.setPosition(selectionEnd_, QTextCursor::MoveMode::KeepAnchor);
    emit selectionStartChanged();
}

int EditorBackend::selectionEnd() const
{
    return m_cursor.selectionEnd();
}

void EditorBackend::setSelectionEnd(int position)
{
    if (position == selectionEnd())
        return;

    m_cursor.setPosition(position, QTextCursor::MoveMode::KeepAnchor);
    emit selectionEndChanged();
}

QString EditorBackend::fontFamily() const
{
    if (m_cursor.isNull())
        return QString();
    QTextCharFormat format = m_cursor.charFormat();
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
    if (m_cursor.isNull())
        return QColor(Qt::black);
    QTextCharFormat format = m_cursor.charFormat();
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
    if (m_cursor.isNull())
        return Qt::AlignLeft;
    return m_cursor.blockFormat().alignment();
}

void EditorBackend::setAlignment(Qt::Alignment alignment)
{
    QTextBlockFormat format;
    format.setAlignment(alignment);
    m_cursor.mergeBlockFormat(format);
    emit alignmentChanged();
}

std::optional<QString> EditorBackend::currentWord()
{
    if (m_cursor.isNull())
        return std::nullopt;

    // Remember original state
    const int originalPosition = m_cursor.position();
    int selectionStart_ = m_cursor.selectionStart();

//    qDebug() << "EditorBackend::currentWord -> Current position: " << m_cursor.position();

    m_cursor.select(QTextCursor::SelectionType::WordUnderCursor);
    if (!m_cursor.hasSelection())
    {
//        qDebug() << "EditorBackend::currentWord -> Current word: No word under cursor";
        return std::nullopt;
    }

    const QString word = m_cursor.selectedText();
    m_cursor.clearSelection();
//    qDebug() << "EditorBackend::currentWord -> Current word: " << word;

    // Restore original state
    if (originalPosition != selectionStart_)
    {
        m_cursor.setPosition(selectionStart_);
        m_cursor.setPosition(originalPosition, QTextCursor::MoveMode::KeepAnchor);
    }
    else
    {
        m_cursor.setPosition(originalPosition);
    }
    return word;
}


bool EditorBackend::hasDecoration(const QString& decorationStart, const QString& decorationEnd)
{

    if (m_cursor.isNull())
        return false;

    bool startsWithDecorator = false;
    bool endsWithDecorator = false;

    if (m_cursor.hasSelection())
    {
        const int originalSelectionStart = m_cursor.selectionStart();
        const int originalSelectionEnd = m_cursor.selectionEnd();
        m_cursor.setPosition(originalSelectionStart);
        m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
        m_cursor.clearSelection();
        m_cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor, decorationStart.length());
        startsWithDecorator = m_cursor.selectedText().startsWith(decorationStart);

        m_cursor.setPosition(originalSelectionEnd);
        m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
        m_cursor.clearSelection();
        m_cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, decorationEnd.length());
        endsWithDecorator = m_cursor.selectedText().startsWith(decorationEnd);

        m_cursor.setPosition(originalSelectionStart);
        m_cursor.setPosition(originalSelectionEnd, QTextCursor::MoveMode::KeepAnchor);
    }
    else
    {
        const int originalPosition = m_cursor.position();
        m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
        m_cursor.movePosition(QTextCursor::MoveOperation::Left, QTextCursor::MoveMode::KeepAnchor, decorationStart.length());
//        qDebug() << "EditorBackend::hasDecoration -> selectedTextStart: " << m_cursor.selectedText();
        startsWithDecorator = m_cursor.selectedText().startsWith(decorationStart);

        m_cursor.setPosition(originalPosition);
        m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
        m_cursor.clearSelection();
        m_cursor.movePosition(QTextCursor::MoveOperation::Right, QTextCursor::MoveMode::KeepAnchor, decorationEnd.length());
//        qDebug() << "EditorBackend::hasDecoration -> selectedTextEnd: " << m_cursor.selectedText();
        endsWithDecorator = m_cursor.selectedText().startsWith(decorationEnd);
        m_cursor.setPosition(originalPosition);
    }

    return startsWithDecorator && endsWithDecorator;
}

bool EditorBackend::decorateCurrentWord(const QString& decorationStart, const QString& decorationEnd)
{

    if (m_cursor.isNull())
        return false;

    const int pos = m_cursor.position();
    m_cursor.beginEditBlock();
    m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    m_cursor.insertText(decorationStart);
    m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    m_cursor.insertText(decorationEnd);
    m_cursor.setPosition(pos + decorationStart.length());
    m_cursor.endEditBlock();
    return true;
}

bool EditorBackend::removeDecorationFromCurrentWord(const QString& decorationStart, const QString& decorationEnd)
{

    if (m_cursor.isNull() || !hasDecoration(decorationStart, decorationEnd))
        return false;

    const int originalPosition = m_cursor.position();

    m_cursor.beginEditBlock();
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> originalPosition = "
//             << m_cursor.position();
    m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> StartOfWord = "
//             << m_cursor.position();
    deletePreviousNChars(&m_cursor, decorationStart.length());
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> currentPosition = "
//             << m_cursor.position();
    m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> EndOfWord = "
//             << m_cursor.position();
    deleteNChars(&m_cursor, decorationEnd.length());
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> currentPosition = "
//             << m_cursor.position();
    m_cursor.setPosition(originalPosition - decorationStart.length());
//    qDebug() << "EditorBackend::removeDecorationFromCurrentWord -> shouldBeOriginalPosition - 2 = "
//             << m_cursor.position();
    m_cursor.endEditBlock();
    return true;
}

bool EditorBackend::decorateCurrentSelection(const QString& decorationStart, const QString& decorationEnd)
{
    if (m_cursor.isNull() || !m_cursor.hasSelection())
        return false;

    const int originalSelectionStart = m_cursor.selectionStart();
    const int originalSelectionEnd = m_cursor.selectionEnd();
    m_cursor.beginEditBlock();
    m_cursor.setPosition(selectionStart());
    m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    m_cursor.insertText(decorationStart);
    m_cursor.setPosition(originalSelectionEnd + decorationStart.length());
    m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    m_cursor.insertText(decorationEnd);
    m_cursor.setPosition(originalSelectionStart + decorationStart.length());
    m_cursor.clearSelection();
    m_cursor.setPosition(originalSelectionEnd + decorationStart.length(), QTextCursor::MoveMode::KeepAnchor);
    m_cursor.endEditBlock();
    return true;
}

bool EditorBackend::removeDecorationFromCurrentSelection(const QString& decorationStart, const QString& decorationEnd)
{
    if (m_cursor.isNull() || !m_cursor.hasSelection() || !hasDecoration(decorationStart, decorationEnd))
        return false;

    const int originalSelectionStart = m_cursor.selectionStart();
    const int originalSelectionEnd = m_cursor.selectionEnd();
    m_cursor.beginEditBlock();
    m_cursor.setPosition(selectionStart());
    m_cursor.movePosition(QTextCursor::MoveOperation::StartOfWord);
    deletePreviousNChars(&m_cursor, decorationStart.length());
    m_cursor.setPosition(originalSelectionEnd - decorationStart.length());
    m_cursor.movePosition(QTextCursor::MoveOperation::EndOfWord);
    deleteNChars(&m_cursor, decorationEnd.length());
    m_cursor.setPosition(originalSelectionStart - decorationStart.length());
    m_cursor.clearSelection();
    m_cursor.setPosition(originalSelectionEnd - decorationStart.length(), QTextCursor::MoveMode::KeepAnchor);
    m_cursor.endEditBlock();
    return true;
}

void EditorBackend::toggleDecoration(const QString& decorationStart, const QString& decorationEnd)
{
    if (m_cursor.isNull())
        return;

    if (m_cursor.hasSelection())
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

bool EditorBackend::bold()
{
    if (m_cursor.isNull())
        return false;

    return hasDecoration("**", "**");
}

void EditorBackend::setBold(bool bold)
{
    if (m_cursor.isNull())
        return;
    if (bold == EditorBackend::bold())
        return;
    toggleDecoration("**", "**");
    emit boldChanged();
}

bool EditorBackend::italic()
{
    if (m_cursor.isNull())
        return false;

    return hasDecoration("_", "_");
}

void EditorBackend::setItalic(bool italic)
{
    if (m_cursor.isNull())
        return;
    if (italic == EditorBackend::italic())
        return;
    toggleDecoration("_", "_");
    emit italicChanged();
}

bool EditorBackend::inlineCode()
{
    if (m_cursor.isNull())
        return false;

    return hasDecoration("`", "`");
}

void EditorBackend::setInlineCode(bool inlineCode)
{
    if (m_cursor.isNull())
        return;
    if (inlineCode == EditorBackend::inlineCode())
        return;
    toggleDecoration("`", "`");
    emit inlineCodeChanged();
}

bool EditorBackend::math()
{
    if (m_cursor.isNull())
        return false;

    return hasDecoration("$", "$");
}

void EditorBackend::setMath(bool math)
{
    if (m_cursor.isNull())
        return;
    if (math == EditorBackend::math())
        return;
    toggleDecoration("$", "$");
    emit inlineCodeChanged();
}

int EditorBackend::fontSize() const
{
    if (m_cursor.isNull())
        return 0;
    QTextCharFormat format = m_cursor.charFormat();
    return format.font().pointSize();
}

void EditorBackend::setFontSize(int size)
{
    if (size <= 0)
        return;

    if (m_cursor.isNull())
        return;

    if (!m_cursor.hasSelection())
        m_cursor.select(QTextCursor::WordUnderCursor);

    if (m_cursor.charFormat().property(QTextFormat::FontPointSize).toInt() == size)
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

void EditorBackend::tabPressed()
{
    if (m_cursor.isNull())
        return;

    const int positionInLine = cursorPositionInLine(&m_cursor);
    const int spacesPerTab = Settings::getInstance()->spacesPerTab();
    const int modPosition = positionInLine % spacesPerTab;
    int numberOfSpaces = spacesPerTab;
    if (modPosition != 0)
    {
        numberOfSpaces = spacesPerTab - modPosition;
    }
    const QString spaceString(" ");
    const QString spaces = spaceString.repeated(numberOfSpaces);
    m_cursor.insertText(spaces);
}

void EditorBackend::addLinkTemplate()
{
    if (m_cursor.isNull())
        return;
    toggleDecoration("[", "]()");
}

bool EditorBackend::cursorIsAt(QTextCursor* m_cursor, const QTextCursor::MoveOperation& moveOp)
{
    if (m_cursor->isNull())
        return false;

    const auto originalPos = m_cursor->position();
    m_cursor->movePosition(moveOp);
    const bool result = originalPos == m_cursor->position();
    m_cursor->setPosition(originalPos);
    return result;
}

int EditorBackend::cursorPositionInLine(QTextCursor *cursor) const
{
    Q_ASSERT(!cursor->isNull());
    const int originalPosition = cursor->position();
    const int originalSelectionStart = cursor->selectionStart();
    cursor->movePosition(QTextCursor::MoveOperation::StartOfLine);
    const int lineStartPosition = cursor->position();
    cursor->setPosition(originalSelectionStart);
    cursor->setPosition(originalPosition, QTextCursor::MoveMode::KeepAnchor);
    return originalPosition - lineStartPosition;
}

void EditorBackend::addCodeBlock()
{
    if (m_cursor.isNull())
        return;
    m_cursor.movePosition(QTextCursor::MoveOperation::EndOfLine);
    m_cursor.insertText("\n```\n```");
    qDebug() << "position before set" << m_cursor.position();
    m_cursor.setPosition(m_cursor.position()-4);
    setCursorPosition(m_cursor.position()-4);
    qDebug() << "position after set" << m_cursor.position();
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

QString EditorBackend::getHTML() const
{
    QTextDocument intermediateDoc{};

    intermediateDoc.setMarkdown(textDocument()->toPlainText());
    return intermediateDoc.toHtml();
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
    return m_cursor;
}

QTextDocument *EditorBackend::textDocument() const
{
    if (!m_document)
        return nullptr;

    return m_document->textDocument();
}

void EditorBackend::mergeFormatOnWordOrSelection(const QTextCharFormat &format)
{
    if (!m_cursor.hasSelection())
        m_cursor.select(QTextCursor::WordUnderCursor);
    m_cursor.mergeCharFormat(format);
}

void EditorBackend::deleteNChars(QTextCursor* m_cursor, int n)
{
    qDebug() << "EditorBackend::deleteNChars -> m_cursor.position() = "
             << m_cursor->position();

    m_cursor->clearSelection();
    qDebug() << "EditorBackend::deleteNChars -> m_cursor.position() = "
             << m_cursor->position();
    m_cursor->setPosition(m_cursor->position() + n, QTextCursor::MoveMode::KeepAnchor);
    qDebug() << "EditorBackend::deleteNChars " << m_cursor->selectedText();
    m_cursor->removeSelectedText();
}

void EditorBackend::deletePreviousNChars(QTextCursor* m_cursor, int n)
{
    qDebug() << "EditorBackend::deletePreviousNChars -> m_cursor.position() = "
             << m_cursor->position();

    m_cursor->clearSelection();
    qDebug() << "EditorBackend::deletePreviousNChars -> m_cursor.position() = "
             << m_cursor->position();
    m_cursor->setPosition(m_cursor->position() - n, QTextCursor::MoveMode::KeepAnchor);
    qDebug() << "EditorBackend::deletePreviousNChars " << m_cursor->selectedText();
    m_cursor->removeSelectedText();
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

