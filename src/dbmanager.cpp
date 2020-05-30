#include "dbmanager.h"
#include "utilities.h"
#include "document.h"

#include <QFileInfo>
#include <QDateTime>

DBManager::DBManager()
    :QObject(nullptr)
{
}


bool DBManager::slotDirectoryLoaded(const QString& directoryPath)
{
    mDB = QSqlDatabase::addDatabase("QSQLITE");

    QString filePath = directoryPath + QString("\\wikiClient.db");
    // Prevent error on existing .db
    while (QFile(filePath).exists())
    {
        auto fileInfo = QFileInfo(filePath);
        filePath = fileInfo.baseName().append("_new.").append(fileInfo.completeSuffix());
    }

    if (!open(filePath))
        return false;

    QSqlQuery q(mDB);

    const QString createTableDocsQuery = "CREATE TABLE Documents ("
                                    "Name           TEXT,"
                                    "Title          TEXT,"
                                    "LastModified	TEXT,"
                                    "Content        TEXT,"
                                    "PRIMARY KEY(Title));";

    const QString createTableLinksQuery = "CREATE TABLE Links ("
                                          "ID INTEGER,"
                                          "Source TEXT,"
                                          "Target TEXT,"
                                          "Display    TEXT,"
                                          "PRIMARY KEY(ID));";

    q.exec(createTableDocsQuery);
    q.exec(createTableLinksQuery);

    return true;
}

bool DBManager::open(const QString& filePath)
{
    if (mDB.isOpen()) {
        if (filePath == mDB.databaseName()) {
            return true;
        }
        else {
            close();
        }
    }

    mDB.setDatabaseName(filePath);

    if(!mDB.open())
    {
        qDebug() << "DBManager::open -> could not open " << filePath;
        return false;
    }
    emit signalDBOpened();
    return true;
}

void DBManager::close()
{
    mDB.close();
    emit signalDBClosed();
}

std::optional<QSqlQuery> DBManager::getQuery()
{
    if (!mDB.isOpen())
        return std::nullopt;
    return QSqlQuery(mDB);
}

void DBManager::slotNewFiles(const QStringList& filePaths)
{
    addDocuments(filePaths);
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
    mDB.transaction();
    // SQL Query to add Document
    QSqlQuery insertDocumentsQuery(mDB);
    QSqlQuery insertLinksQuery(mDB);

    insertDocumentsQuery.prepare("INSERT INTO Documents (Name, Title, LastModified, Content) "
                                 "VALUES (:name, :title, :lastModified, :content)");

    insertLinksQuery.prepare("INSERT INTO Links (Source, Target, Display) "
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

    mDB.commit();
}
