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

bool DBManager::createNewDatabase(const QString& filePath)
{
    if (!connectToDatabase(filePath))
        return false;

    QSqlQuery q(m_db);

    QStringList createTableQueries;
    createTableQueries << ""
        "CREATE TABLE documents(id  INTEGER PRIMARY KEY,"
        "name           TEXT,"
        "title          TEXT,"
        "last_modified	TEXT,"
        "content        TEXT,"
        "UNIQUE(name));";

    createTableQueries << ""
        "CREATE TABLE links ("
        "id INTEGER PRIMARY KEY,"
        "source TEXT,"
        "target TEXT,"
        "display    TEXT,"
        "type    TEXT,"
        "UNIQUE(Source, Target));";

    createTableQueries << ""
        "CREATE VIRTUAL TABLE fts_documents "
        "USING fts5(name, title, content, content='documents', content_rowid='id');";

    createTableQueries << ""
        "CREATE TRIGGER remove_from_fts_before_insert_if_name_exists  "
        "BEFORE INSERT ON documents "
        "WHEN EXISTS(SELECT name FROM fts_documents WHERE name=new.name) "
        "BEGIN "
            "INSERT INTO fts_documents(fts_documents, rowid, name, title, content)  "
            "VALUES('delete', (SELECT rowid FROM fts_documents WHERE name=new.name),  new.name, new.title, new.content); "
        "END; ";

    createTableQueries << ""
        "CREATE TRIGGER add_to_fts_after_insert  "
        "AFTER INSERT ON documents "
        "BEGIN "
            "INSERT INTO fts_documents(rowid, name, title, content) "
            "VALUES(new.id, new.name, new.title, new.content); "
        "END;  ";

    createTableQueries << ""
        "CREATE TRIGGER remove_from_fts_after_delete  "
        "AFTER DELETE ON documents "
        "BEGIN "
            "INSERT INTO fts_documents(fts_documents, rowid, name, title, content)  "
            "VALUES('delete', old.id,  old.name, old.title, old.content); "
        "END; ";

    createTableQueries << ""
        "CREATE TRIGGER Documents_au AFTER UPDATE ON Documents BEGIN "
            "INSERT INTO fts_Documents(fts_Documents, Name, Title, Content) VALUES('delete', old.Name, old.Title, old.Content); "
            "INSERT INTO fts_Documents(Name, Title, Content) VALUES (new.Name, new.Title, new.Content); "
        "END;";

    for (const QString query: createTableQueries)
    {
        if (!q.exec(query))
        qDebug() << "DBManager::createNewDatabase -> Query failed: " << query;
    }
    m_dbEmpty = true;
    return true;
}

void DBManager::addDocuments(const QStringList& filePaths)
{
    m_db.transaction();
    // SQL Query to add Document
    QSqlQuery insertDocumentsQuery(m_db);
    QSqlQuery insertLinksQuery(m_db);

    insertDocumentsQuery.prepare("REPLACE INTO documents (name, title, last_modified, content) "
                                 "VALUES (:name, :title, :lastModified, :content)");

    insertLinksQuery.prepare("INSERT OR REPLACE INTO links (source, target, display, type) "
                             "VALUES (:source, :target, :display, :type)");

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
        insertDocumentsQuery.bindValue(":last_modified", lastModified.toString());
        insertDocumentsQuery.bindValue(":content", doc->content);
        if (!insertDocumentsQuery.exec())
            qDebug() << "DBManager::addDocuments -> Query failed" << insertDocumentsQuery.lastQuery()
                     << "with Error" << insertDocumentsQuery.lastError();

        for (auto link: doc->links)
        {
            insertLinksQuery.bindValue(":source", link.source);
            insertLinksQuery.bindValue(":target", link.target);
            insertLinksQuery.bindValue(":display", link.display);
            insertLinksQuery.bindValue(":type", QMetaEnum::fromType<Document::LinkType>().valueToKey(int(link.linkType)));
            if (!insertLinksQuery.exec())
                qDebug() << "DBManager::addDocuments -> Query failed" << insertLinksQuery.lastQuery()
                         << "with Error" << insertLinksQuery.lastError();
        }
    }

    m_db.commit();

    QSqlQuery vacuumQuery(m_db);
    vacuumQuery.exec("VACUUM;");

    if (m_dbEmpty)
    {
        emit signalDBInitialized();
        m_dbEmpty = false;
    }
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

void DBManager::initializeTableModel()
{
    m_tableModel.reset(new QSqlTableModel(this, m_db));
}
