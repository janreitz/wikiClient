#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>
#include <QFileSystemModel>

class FileManager : public QFileSystemModel
{
    Q_OBJECT
    Q_PROPERTY(QString workingDirectory READ workingDirectory WRITE setWorkingDirectory NOTIFY workingDirectoryChanged)

public:
    explicit FileManager();

    Q_INVOKABLE QModelIndex getCurrentPathIndex();

    QString workingDirectory() const;
    void setWorkingDirectory(const QString&);

public slots:
    // Recursively iterate through dirPath updating the internal representation,
    // and watching all files
    void slotScanDirectory();
    void slotFileChanged(const QString& filePath);
    void slotDirChanged(const QString& dirPath);

signals:
    void signalNewFiles(const QStringList& filePaths);
    void signalFileModified(const QString& filePath);
    void signalFilesDeleted(const QStringList& filePaths);
    void workingDirectoryChanged();

private:
    QFileSystemWatcher mFileWatcher;
    QHash<QString, QFileInfo> mFileInfosByPaths;
};

#endif // FILEMANAGER_H
