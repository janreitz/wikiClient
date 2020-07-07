#include <QtTest/QtTest>
#include "../../src/dbmanager.h"

#include <QDir>

class tst_DBManager : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void tst_createNewDatabase();
    void tst_updateDocumentEntry();
    void tst_addDocuments();
    void cleanupTestCase();

private:
    bool removeDir(const QString& dirPath);
    DBManager* m_dbManager;
    QDir m_testingDir;
};
