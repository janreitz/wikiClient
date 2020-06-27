#include "gitmanager.h"
#include "settings.h"
#include "qgitglobal.h"

GitManager* GitManager::m_instance = nullptr;

GitManager::GitManager(QObject* parent)
    : QObject(parent)
{
    LibQGit2::initLibQGit2();
    LibQGit2::Repository m_repo{};
}

GitManager::~GitManager()
{
    LibQGit2::shutdownLibQGit2();
}

GitManager* GitManager::getInstance()
{
    if (!m_instance)
    {
        m_instance = new GitManager();
    }
    return m_instance;
}

void GitManager::gitInit()
{
    return gitInit(Settings::getInstance()->rootDirectory());
}

void GitManager::gitInit(const QDir &directory)
{
    if (!directory.exists())
    {
        emit error("GitManager::gitInit -> Cannot init, directory does not exist");
    }

    m_repo.init(directory.path());

//    git_repository* m_repo;
//    const int returnCode = git_repository_init(&m_repo, directory.path().toLocal8Bit(), false);
//    if (returnCode != 0)
//    {
//        emit error("GitManager::gitInit -> init failed with error code: " + QString(returnCode));
//    }
}
