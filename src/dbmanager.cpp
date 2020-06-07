#include "dbmanager.h"
#include "utilities.h"
#include "document.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDir>

DBManager::DBManager()
    :QObject(nullptr)
    , m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
}


bool DBManager::slotRootDirectoryChanged(const QString& directoryPath)
{
    close();

    auto initialFilePath = directoryPath + QDir::separator() + "wikiClient.db";
    auto fileInfo = QFileInfo(initialFilePath);
    auto filePath = fileInfo.absoluteFilePath();

    if (fileInfo.exists()) {
        return connectToDatabase(filePath);
    }
    else {
        return createNewDatabase(filePath);
    }

//    // Prevent error on existing .db
//    while (QFile(filePath).exists())
//    {
//        auto fileInfo = QFileInfo(filePath);
//        filePath = fileInfo.canonicalPath()
//                + "/"
//                + fileInfo.baseName()
//                + "_new."
//                + fileInfo.completeSuffix();
//    }
//    return createNewDatabase(filePath);
}

bool DBManager::createNewDatabase(const QString& filePath)
{
    //mDB = QSqlDatabase::addDatabase("QSQLITE");

    if (!connectToDatabase(filePath))
        return false;

    QSqlQuery q(m_db);

    const QString createTableDocsQuery = "CREATE TABLE Documents ("
                                    "Name           TEXT,"
                                    "Title          TEXT,"
                                    "LastModified	TEXT,"
                                    "Content        TEXT,"
                                    "PRIMARY KEY(Name));";

    const QString createTableLinksQuery = "CREATE TABLE Links ("
                                          "ID INTEGER,"
                                          "Source TEXT,"
                                          "Target TEXT,"
                                          "Display    TEXT,"
                                          "PRIMARY KEY(ID),"
                                          "UNIQUE(Source, Target));";

    q.exec(createTableDocsQuery);
    q.exec(createTableLinksQuery);

    return true;
}

bool DBManager::connectToDatabase(const QString& filePath)
{
    //mDB = QSqlDatabase::addDatabase("QSQLITE");

    if (m_db.isOpen()) {
        if (filePath == m_db.databaseName()) {
            return true;
        }
        else {
            close();
        }
    }

    m_db.setDatabaseName(filePath);

    if(!m_db.open())
    {
        qDebug() << "DBManager::connectToDatabase -> could not open "
                 << filePath
                 << " QSqlError: "
                 << m_db.lastError().text();
        return false;
    }

    //initializeTableModel();

    emit signalDBOpened();
    return true;
}

void DBManager::close()
{
    m_db.close();
    emit signalDBClosed();
}

std::optional<QSqlQuery> DBManager::getQuery()
{
    if (!m_db.isOpen())
        return std::nullopt;
    return QSqlQuery(m_db);
}

//std::shared_ptr<QSqlTableModel> DBManager::tableModel()
//{
//    if (!m_db.isOpen())
//        return nullptr;

//    return m_tableModel;
//}

void DBManager::slotNewFiles(const QStringList& filePaths)
{
    QStringList filteredFilePaths;

    for (auto filePath : filePaths)
    {
        // Start writing good code: prevent unnecessary copy on assignment
        auto fileInfo = std::make_unique<QFileInfo>(QFileInfo(filePath));

        auto suffix = fileInfo->completeSuffix();

        if (fileInfo->exists() && fileInfo->completeSuffix() == "md")
        {
            filteredFilePaths << filePath;
        }
    }

    addDocuments(filteredFilePaths);
}
void DBManager::slotFileRenamed(const QString &path, const QString &oldName, const QString &newName)
{
    // SQL Query to change file name
    Q_UNUSED(path)
    Q_UNUSED(oldName)
    Q_UNUSED(newName)
}
void DBManager::slotFileModified(const QString& filePath)
{
    // SQL Query to update contents
    Q_UNUSED(filePath)
}
void DBManager::slotFilesDeleted(const QStringList& filePaths)
{
    // SQL Query to delete Document
    Q_UNUSED(filePaths)
}
void DBManager::addDocuments(const QStringList& filePaths)
{
    m_db.transaction();
    // SQL Query to add Document
    QSqlQuery insertDocumentsQuery(m_db);
    QSqlQuery insertLinksQuery(m_db);

    insertDocumentsQuery.prepare("REPLACE INTO Documents (Name, Title, LastModified, Content) "
                                 "VALUES (:name, :title, :lastModified, :content)");

    insertLinksQuery.prepare("INSERT OR IGNORE INTO Links (Source, Target, Display) "
                             "VALUES (:source, :target, :display)");

    for (auto filePath : filePaths)
    {
        auto fileInfo = QFileInfo(filePath);
        auto lastModified = fileInfo.lastModified();

        auto content = Utilities::stringFromFile(filePath);
        if (!content) { continue;}

        // parse title and links
        auto doc = Document::fromString(fileInfo.baseName(), *content);
        if (!doc) {continue;}
        insertDocumentsQuery.bindValue(":name", doc->name);
        insertDocumentsQuery.bindValue(":title", doc->title);
        insertDocumentsQuery.bindValue(":lastModified", lastModified.toString());
        insertDocumentsQuery.bindValue(":content", doc->content);
        insertDocumentsQuery.exec();

        for (auto link: doc->links)
        {
            insertLinksQuery.bindValue(":source", link.source);
            insertLinksQuery.bindValue(":target", link.target);
            insertLinksQuery.bindValue(":display", link.display);
            insertLinksQuery.exec();
        }
    }

    m_db.commit();
}

//void DBManager::initializeTableModel()
//{
//    m_tableModel = std::make_shared<QSqlTableModel>(this, m_db);
//}
