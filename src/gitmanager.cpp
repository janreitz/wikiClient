#include "gitmanager.h"
#include "settings.h"
#include "qgitglobal.h"
#include "qgitexception.h"

#include <QDebug>
#include <QDirIterator>

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

void GitManager::init()
{
    return init(Settings::getInstance()->rootDirectory());
}

void GitManager::init(const QDir &directory)
{
    if (!directory.exists())
    {
        emit error("GitManager::gitInit -> Cannot init, directory does not exist");
    }

    m_repo.init(directory.path());


}

void GitManager::add(const QStringList& filePaths)
{
    auto index = m_repo.index();
    for (auto filePath: filePaths)
    {
        const QFile file(filePath);
        const QDir workingDir(m_repo.workDirPath());
        if (file.exists()) // Todo: check if file in dir
            index.addByPath(filePath);
    }
}

void GitManager::addAll()
{
    auto index = m_repo.index();
    const auto cwd = m_repo.workDirPath();
    qDebug() << cwd;
    QDirIterator workingDirIter(cwd,
                                QStringList() << "*.md",
                                QDir::NoDotAndDotDot | QDir::Files | QDir::Readable,
                                QDirIterator::Subdirectories);

    while (workingDirIter.hasNext())
    {
        workingDirIter.next();
        try {
            index.addByPath(workingDirIter.fileName());
        } catch (LibQGit2::Exception& e) {
            qDebug() << "GitManager::addAll -> Exception caught: "
                     << e.message();
        }

    }

    index.updateAll();
    try {
        index.write();
    } catch (LibQGit2::Exception& e) {
        qDebug() << "GitManager::addAll -> Exception caught: "
                 << e.message();
    }
}

void GitManager::commit(const QString& message)
{
    LibQGit2::StatusOptions statusOptions;
    auto status = m_repo.status(statusOptions);

    if (status.entryCount() == 0)
        return;

    // TODO read this info from an existing installation
    LibQGit2::Signature committer{"Jan Reitz", "jan-reitz@gmx.de"};
    LibQGit2::Signature author{"Jan Reitz", "jan-reitz@gmx.de"};
    auto treeOid = m_repo.index().createTree();
    auto tree = m_repo.lookupTree(treeOid);

    QList<LibQGit2::Commit> parentCommits;

    if (m_repo.isEmpty())
    {
        // parent commits can be empty for the initial commit
    }
    else
    {
        try {
            auto headOid =  m_repo.head().target();
            parentCommits << m_repo.lookupCommit(headOid);
        } catch (LibQGit2::Exception& e) {
            qDebug() << "GitManager::commit -> Exception caught: "
                     << e.message();
        }
    }
    try {
        auto newCommitId = m_repo.createCommit(tree, parentCommits, author, committer, message);
        m_repo.createTag("HEAD", m_repo.lookupAny(newCommitId));
    } catch (LibQGit2::Exception& e) {
        qDebug() << "GitManager::commit -> Exception caught: "
                 << e.message();
    }
}

void GitManager::slotWorkingDirectoryChanged(const QString &newWorkingDirectory)
{
    try {
        m_repo.discoverAndOpen(newWorkingDirectory, false, QStringList(newWorkingDirectory));

    } catch (LibQGit2::Exception& e) {
        qDebug() << "GitManager::slotWorkingDirectoryChanged -> Exception caught: "
                 << e.message()
                 << " -> Initiating new repo";
        init(newWorkingDirectory);
    }
}
