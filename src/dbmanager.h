#ifndef DBHANDLER_H
#define DBHANDLER_H

#include <QQuickItem>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlTableModel>

#include <optional>

class DBManager : public QObject
{
    Q_OBJECT
    //Q_PROPERTY(QSqlTableModel* tableModel READ tableModel NOTIFY tableModelChanged)

public:
    DBManager();

    bool connectToDatabase(const QString& filePath);
    void writeToFile(const QString& filePath);
    void close();
    bool isOpen();
    // return read-only query
    std::optional<QSqlQuery> getQuery();
    std::optional<std::shared_ptr<QSqlTableModel>> tableModel();

public slots:
    bool slotRootDirectoryChanged(const QString& directoryPath);
    void slotNewFiles(const QStringList& filePaths);
    void slotFileRenamed(const QString &path, const QString &oldName, const QString &newName);
    void slotFileModified(const QString& filePath);
    void slotFilesDeleted(const QStringList& filePaths);

signals:
    void signalDBOpened();
    void signalDBClosed();
    void tableModelChanged();

private:

    bool createNewDatabase(const QString& filePath);
    bool updateDocumentEntry();
    void addDocuments(const QStringList& filePaths);
    void initializeTableModel();

    QSqlDatabase m_db;
    bool tableModelIsInitialized;
    std::shared_ptr<QSqlTableModel> m_tableModel;
};

#endif // DBHANDLER_H
