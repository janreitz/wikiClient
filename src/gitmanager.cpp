#include "gitmanager.h"
#include "settings.h"
#include "utilities.h"

#include "qgitglobal.h"
#include "qgitexception.h"
#include "qgitoid.h"
#include "private/pathcodec.h"

#include "git2/index.h"

#include <QDebug>
#include <QDirIterator>

GitManager* GitManager::m_instance = nullptr;

GitManager::GitManager(QObject* parent)
    : QObject(parent)
    , m_committer("Jan Reitz", "jan-reitz@gmx.de")
    , m_author("Jan Reitz", "jan-reitz@gmx.de")
    , m_repo()
{
}

void GitManager::addHEADCommitIfNotBare(QList<LibQGit2::Commit> *parentCommits) const
{
    if (m_repo.isEmpty())
    {
        // parent commits can be empty for the initial commit
    }
    else
    {
        try {
            auto headOid =  m_repo.head().target();
            parentCommits->append(m_repo.lookupCommit(headOid));
        } catch (LibQGit2::Exception& e) {
            qDebug() << "GitManager::addHEADCommitIfNotBare -> Exception caught: "
                     << e.message();
        }
    }
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

void GitManager::add(const QStringList& fileNames)
{
    auto index = m_repo.index();
    const auto cwd = m_repo.workDirPath();
    for (auto fileName: fileNames)
    {
        if (QFile::exists(cwd + fileName))
        {
            try {
                index.addByPath(fileName);
            } catch (LibQGit2::Exception& e) {
                qDebug() << "GitManager::add -> Exception caught: "
                         << e.message();
            }
        }
    }
    try {
        index.write();
    } catch (LibQGit2::Exception& e) {
        qDebug() << "GitManager::addAll -> Exception caught: "
                 << e.message();
    }
}

void GitManager::addAll()
{
    const auto cwd = m_repo.workDirPath();
    // qDebug() << cwd;
    QDirIterator workingDirIter(cwd,
                                QStringList() << "*.md",
                                QDir::NoDotAndDotDot | QDir::Files | QDir::Readable,
                                QDirIterator::Subdirectories);

    QStringList fileNames;
    while (workingDirIter.hasNext())
    {
        workingDirIter.next();
        fileNames << workingDirIter.fileName();
    }
    add(fileNames);
}

void GitManager::commit(const QString& message)
{
    LibQGit2::StatusOptions statusOptions;
    auto status = m_repo.status(statusOptions);

    if (status.entryCount() == 0)
        return;

    if (m_repo.index().hasConflicts())
    {
        qDebug() << "GitManager::commit -> Index contains conflicts, cannot commit";
        return;
    }
    auto treeOid = m_repo.index().createTree();
    auto tree = m_repo.lookupTree(treeOid);

    QList<LibQGit2::Commit> parentCommits;
    addHEADCommitIfNotBare(&parentCommits);

    try {
        auto newCommitId = m_repo.createCommit(tree, parentCommits, m_author, m_committer, message, "HEAD");
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

void GitManager::slotFileSaved(const QString &filePath)
{
    auto relativePath = Utilities::getRelativePathTo(m_repo.workDirPath(), filePath);
    if (relativePath)
        addAndCommitFile(*relativePath);
}

void GitManager::addAndCommitFile(const QString & fileName)
{
    add(QStringList() << fileName);
    // I don't like, that this might unintended commit other staged changes,
    // but my try at cirumventing this was not successful (below)
    commit("Updated " + fileName);

    // Creates an in-memory index, adds only the requested file to it,
    // creates a tree object from this index and commits it.
    // This way the existing index is not cleared and no files are unintendedly committed

//    LibQGit2::OId oid{};
//    // Low-level operation, because LibQGit2 doesn't provide
//    // the necessary interface to initialize a pure in-memory index
//    auto index = m_repo.index().data();
//    // create new in-memory index
//    git_index_new(&index);
//    try {
//        // LibQGit2 private PathCodex::toLibGit2 just does QString::toUtf8
//        git_index_add_bypath(index, QString(m_repo.workDirPath() + fileName).toUtf8());
//    } catch (LibQGit2::Exception& e) {
//        qDebug() << "GitManager::commitFile -> Exception caught: "
//                 << e.message();
//        return;
//    }
//    // Low-level operation, because LibQGit2 doesn't provide the necessary interface
//    git_index_write_tree_to(oid.data(), index, m_repo.data());
//    git_index_free(index);
//    // Reset the in-memory index to match the index file
//    git_repository_index(&index, m_repo.data());

//    auto tree = m_repo.lookupTree(oid);

//    QList<LibQGit2::Commit> parentCommits;
//    addHEADCommitIfNotBare(&parentCommits);

//    try {
//        m_repo.createCommit(tree, parentCommits, m_author, m_committer, "Updated " + fileName, "HEAD");
//    } catch (LibQGit2::Exception& e) {
//        qDebug() << "GitManager::CommitFile -> Exception caught: "
//                 << e.message();
//    }
}
