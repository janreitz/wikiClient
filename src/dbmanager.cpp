#include "dbmanager.h"
#include "utilities.h"
#include "document.h"

#include <QFileInfo>
#include <QDateTime>
#include <QDir>

DBManager* DBManager::m_instance = nullptr;

DBManager::DBManager()
    :QObject(nullptr)
    , m_db(QSqlDatabase::addDatabase("QSQLITE"))
{
}

DBManager *DBManager::getInstance()
{
    if (!m_instance) {
        DBManager::m_instance = new DBManager();
    }
    return m_instance;
}

void DBManager::slotWorkingDirectoryChanged(const QString& directoryPath)
{
    close();

    auto initialFilePath = directoryPath + QDir::separator() + "wikiClient.db";
    auto fileInfo = QFileInfo(initialFilePath);
    auto filePath = fileInfo.absoluteFilePath();

    if (fileInfo.exists()) {
        if (connectToDatabase(filePath))
        {
            emit signalDBAvailable();
        }
    }
    else
    {
        if (createNewDatabase(filePath))
        {
            emit signalDBAvailable();
        }
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
    const QString createTableFTSQuery = "CREATE VIRTUAL TABLE fts_Documents USING fts5(Name, Title, Content);";
    const QString createTriggerInsertFTSQuery = ""
          "CREATE TRIGGER Documents_ai AFTER INSERT ON Documents BEGIN "
            "INSERT INTO fts_Documents(Name, Title, Content) VALUES (new.Name, new.Title, new.Content); "
          "END; ";
    const QString createTriggerDeleteFTSQuery = ""
          "CREATE TRIGGER Documents_ad AFTER DELETE ON Documents BEGIN "
            "INSERT INTO fts_Documents(fts_Documents, Name, Title, Content) VALUES('delete', old.Name, old.Title, old.Content); "
          "END; ";
    const QString createTriggerUpdateFTSQuery = ""
          "CREATE TRIGGER Documents_au AFTER UPDATE ON Documents BEGIN "
            "INSERT INTO fts_Documents(fts_Documents, Name, Title, Content) VALUES('delete', old.Name, old.Title, old.Content); "
            "INSERT INTO fts_Documents(Name, Title, Content) VALUES (new.Name, new.Title, new.Content); "
          "END;";

    if (!q.exec(createTableDocsQuery))
        qDebug() << "DBManager::createNewDatabase -> Query failed: " << createTableDocsQuery;
    if (!q.exec(createTableLinksQuery))
        qDebug() << "DBManager::createNewDB -> Query failed: " << createTableLinksQuery;
    if (!q.exec(createTableFTSQuery))
        qDebug() << "DBManager::createNewDB -> Query failed: " << createTableFTSQuery;
    if (!q.exec(createTriggerInsertFTSQuery))
        qDebug() << "DBManager::createNewDB -> Query failed: " << createTriggerInsertFTSQuery
                 << " with Error: " << q.lastError();
    if (!q.exec(createTriggerDeleteFTSQuery))
        qDebug() << "DBManager::createNewDB -> Query failed: " << createTriggerDeleteFTSQuery
                 << " with Error: " << q.lastError();
    if (!q.exec(createTriggerUpdateFTSQuery))
        qDebug() << "DBManager::createNewDB -> Query failed: " << createTriggerUpdateFTSQuery
                 << " with Error: " << q.lastError();

    m_dbEmpty = true;
    return true;
}

bool DBManager::connectToDatabase(const QString& filePath)
{
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

    initializeTableModel();
    return true;
}

void DBManager::close()
{
    if (m_db.isOpen())
    {
        m_db.close();
        emit signalDBClosed();
    }
}

bool DBManager::isOpen()
{
    return m_db.isOpen();
}

std::optional<QSqlQuery> DBManager::getQuery()
{
    if (!m_db.isOpen())
        return std::nullopt;
    return QSqlQuery(m_db);
}

std::optional<std::shared_ptr<QSqlTableModel>> DBManager::tableModel()
{
    if (!m_db.isOpen())
        return std::nullopt;

    return m_tableModel;
}

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
        if (!insertDocumentsQuery.exec())
            qDebug() << "DBManager::addDocuments -> Query failed" << insertDocumentsQuery.lastQuery()
                     << "with Error" << insertDocumentsQuery.lastError();

        for (auto link: doc->links)
        {
            insertLinksQuery.bindValue(":source", link.source);
            insertLinksQuery.bindValue(":target", link.target);
            insertLinksQuery.bindValue(":display", link.display);
            if (!insertLinksQuery.exec())
                qDebug() << "DBManager::addDocuments -> Query failed" << insertLinksQuery.lastQuery()
                         << "with Error" << insertLinksQuery.lastError();
        }
    }

    m_db.commit();
    if (m_dbEmpty)
    {
        emit signalDBInitialized();
        m_dbEmpty = false;
    }
}

void DBManager::initializeTableModel()
{
    m_tableModel.reset(new QSqlTableModel(this, m_db));
}
