#include "tst_dbmanager.h"
#include "../../src/dbmanager.h"

#include <QCoreApplication>
#include <QDir>

void tst_DBManager::initTestCase()
{
    m_testingDir.setPath(QCoreApplication::applicationDirPath());
    m_testingDir.mkdir("testing_dir");
    m_testingDir.cd("testing_dir");
    m_dbManager = DBManager::getInstance();
}

void tst_DBManager::tst_createNewDatabase()
{
    m_dbManager->slotWorkingDirectoryChanged(m_testingDir.path());
    QVERIFY(m_testingDir.exists("wikiClient.db"));
}

void tst_DBManager::tst_addDocuments()
{

}

void tst_DBManager::tst_updateDocumentEntry()
{

}

void tst_DBManager::cleanupTestCase()
{
    // Remove database file
    // m_testingDir.cd("..");

    m_dbManager->close();
    QVERIFY(m_testingDir.removeRecursively());
}

QTEST_MAIN(tst_DBManager)
