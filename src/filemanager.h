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
    Q_PROPERTY(QString rootPath READ rootPath WRITE setRootPath NOTIFY rootPathChanged)

public:
    explicit FileManager();

    Q_INVOKABLE QModelIndex getCurrentPathIndex();

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
    void rootPathChanged();

private:
    QFileSystemWatcher mFileWatcher;
    QHash<QString, QFileInfo> mFileInfosByPaths;
};

#endif // FILEMANAGER_H
