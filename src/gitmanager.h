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

    void gitInit(const QDir& directory);
    // Initialize empty git repository in current working directory
    void gitInit();

signals:
    void error(const QString& errorMessage);

private:
    static GitManager* m_instance;

    explicit GitManager(QObject* parent = nullptr);
    // Initialize empty git repository in given directory

    LibQGit2::Repository m_repo;
};

#endif // GITMANAGER_H
