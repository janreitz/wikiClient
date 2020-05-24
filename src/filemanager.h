#ifndef FILEMANAGER_H
#define FILEMANAGER_H

#include <QObject>
#include <QFileInfo>
#include <QDir>
#include <QFileSystemWatcher>

class FileManager : public QObject
{
    Q_OBJECT
public:
    explicit FileManager();

public slots:
    void slotSetDirectory(QString dirPath);
    // Recursively iterate through dirPath updating the internal representation,
    // and watching all files
    void slotScanDirectory();
    void slotFileChanged(const QString& filePath);
    void slotDirChanged(const QString& dirPath);

signals:
    void signalNewWorkingDirectory(const QString& newDirectoryPath);
    void signalNewFiles(const QStringList& filePaths);
    void signalFileRenamed(const QString& oldFilePath, const QString& newFilePath);
    void signalFileModified(const QString& filePath);
    void signalFilesDeleted(const QStringList& filePaths);

private:

    QDir mDir;
    QFileSystemWatcher mFileWatcher;
    QHash<QString, QFileInfo> mFileInfosByPaths;
};

#endif // FILEMANAGER_H
