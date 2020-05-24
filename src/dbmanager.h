#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QQuickItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>

class DBManager : public QObject
{
    Q_OBJECT
public:
    DBManager();

    bool open(const QString& filePath);
    void writeToFile(const QString& filePath);
    void close();
    // return read-only query
    std::optional<QSqlQuery> getQuery();

public slots:
    bool slotCreateNewDB(const QString& directoryPath);
    void slotNewFiles(const QStringList& filePaths);
    void slotFileRenamed(const QString& oldFilePath, const QString& newFilePath);
    void slotFileModified(const QString& filePath);
    void slotFilesDeleted(const QStringList& filePaths);

private:

    bool updateDocumentEntry();
    void addDocuments(const QStringList& filePaths);

    QSqlDatabase mDB;
};

#endif // DBHANDLER_H
