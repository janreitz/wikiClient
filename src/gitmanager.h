#ifndef GITMANAGER_H
#define GITMANAGER_H

#include <QObject>
#include <QDir>

#include "qgitrepository.h"

class GitManager: public QObject
{
    Q_OBJECT

public:
    static GitManager* getInstance();
    ~GitManager();

    void init(const QDir& directory);
    // Initialize empty git repository in current working directory
    void init();

    Q_INVOKABLE void add(const QStringList& filePaths);
    Q_INVOKABLE void addAll();
    Q_INVOKABLE void commit(const QString& message);
    Q_INVOKABLE void addAndCommitFile(const QString& fileName);

public slots:
    void slotWorkingDirectoryChanged(const QString& newWorkingDirectory);
    void slotFileSaved(const QString& filePath);

signals:
    void error(const QString& errorMessage);

private:
    static GitManager* m_instance;

    explicit GitManager(QObject* parent = nullptr);

    // returns empty
    void addHEADCommitIfNotBare(QList<LibQGit2::Commit>* parentCommits) const;

    LibQGit2::Signature m_committer;
    LibQGit2::Signature m_author;
    LibQGit2::Repository m_repo;
};

#endif // GITMANAGER_H
