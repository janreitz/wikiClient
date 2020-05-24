#include "filemanager.h"

#include <QDirIterator>
#include <QDebug>
#include <QSet>

FileManager::FileManager()
{
    connect(&mFileWatcher, &QFileSystemWatcher::fileChanged, this, &FileManager::slotFileChanged);
    connect(&mFileWatcher, &QFileSystemWatcher::directoryChanged, this,&FileManager::slotFileChanged);
}

void FileManager::slotSetDirectory(QString dirPath)
{
    if (mDir.path() == dirPath) {return;}
    auto newDir = QDir(dirPath);
    if (newDir.exists())
    {
        mDir.setPath(dirPath);
        emit signalNewWorkingDirectory(dirPath);
        return;
    }
    qDebug() << "FileManager::slotSetDirectory -> Directory doesn't exist";
}

// This slot is called when the file at the specified path is modified,
// renamed or removed from disk.
void FileManager::slotFileChanged(const QString& filePath)
{
    auto changedFileInfo = QFileInfo(filePath);
    // Find out whether the file has been modified, renamed or removed
    if (mFileInfosByPaths.contains(filePath))
    {
        if (mFileInfosByPaths[filePath] != changedFileInfo)
        {
            // Modified!
            emit signalFileModified(filePath);
            mFileInfosByPaths[filePath] = changedFileInfo;
        }
        else
        {
            // false alarm?
            return;
        }
    }
    else
    {
        auto actualFilePaths = mDir.entryList();

        auto actualFilePathsSet = QSet<QString>(actualFilePaths.begin(), actualFilePaths.end());
        auto storedFilePathsSet = QSet<QString>(mFileInfosByPaths.keyBegin(), mFileInfosByPaths.keyEnd());


        if (actualFilePathsSet.size() == storedFilePathsSet.size())
        {
            // Renamed!
            auto oldNameSet = storedFilePathsSet - actualFilePathsSet;
            auto newNameSet = actualFilePathsSet - storedFilePathsSet ;

            Q_ASSERT(oldNameSet.size()==1);
            Q_ASSERT(newNameSet.size()==1);

            // Returns a non-const STL-style iterator positioned at the first item in the set.
            emit signalFileRenamed(*oldNameSet.begin(), *newNameSet.begin());
        }
        else
        {
            // Removed!
            auto removedFileSet = storedFilePathsSet - actualFilePathsSet;
            if (removedFileSet.size()==1)
            {
                emit signalFilesDeleted(QStringList(*removedFileSet.begin()));
            }
            else
            {
                // Something got out of sync, rescan the whole directory
                // This should never happen!
                slotScanDirectory();
            }
        }
    }
}

// This signal is emitted when the directory at a specified path is modified
// (e.g., when a file is added or deleted) or removed from disk.
void FileManager::slotDirChanged(const QString& dirPath)
{
    // Find out whether a file has been added or deleted, or if the directory has been deleted
    Q_UNUSED(dirPath)
}


void FileManager::slotScanDirectory()
{
    if(!mDir.exists())
        return;

    QStringList newFiles;
    QStringList deletedFiles;

    QSet<QString> storedFilePaths(mFileInfosByPaths.keyBegin(), mFileInfosByPaths.keyEnd());
    QSet<QString> actualFilePaths;

    auto iter = QDirIterator(mDir.path(), QDirIterator::Subdirectories);
    while (iter.hasNext())
    {
        const QString filePath = iter.next();
        actualFilePaths.insert(filePath);

        if (!mFileInfosByPaths.contains(filePath))
        {
            newFiles << filePath;
        }
        else
        {
            if (mFileInfosByPaths[filePath] != iter.fileInfo())
            {
                emit signalFileModified(filePath);
            }
        }
        mFileInfosByPaths.insert(filePath, iter.fileInfo());
        // idempotent call
        mFileWatcher.addPath(filePath);

    }

    for (auto deletedFilePath : storedFilePaths - actualFilePaths)
    {
        deletedFiles << deletedFilePath;
        mFileWatcher.removePath(deletedFilePath);
    }

    if (!newFiles.isEmpty())
    {
        emit signalNewFiles(newFiles);
    }
    if (!deletedFiles.isEmpty())
    {
        emit signalFilesDeleted(deletedFiles);
    }
    return;
}

