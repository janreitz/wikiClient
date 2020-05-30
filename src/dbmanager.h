#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QQuickItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlQueryModel>

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
    //std::optional<QSqlQueryModel*> getQueryModel();

public slots:
    bool slotRootPathChanged(const QString& directoryPath);
    void slotNewFiles(const QStringList& filePaths);
    void slotFileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void slotFileModified(const QString& filePath);
    void slotFilesDeleted(const QStringList& filePaths);

signals:
    void signalDBOpened();
    void signalDBClosed();

private:

    bool updateDocumentEntry();
    void addDocuments(const QStringList& filePaths);

    QSqlDatabase mDB;
};

#endif // DBHANDLER_H
