#include "linksuggestionprovider.h"
#include "filemanager.h"
#include "dbmanager.h"

#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    FileManager theFileManager;
    DBManager theDBManager;

    QObject::connect(&theFileManager, &FileManager::signalNewFiles, &theDBManager, &DBManager::slotNewFiles);
    QObject::connect(&theFileManager, &FileManager::signalFileRenamed, &theDBManager, &DBManager::slotFileRenamed);
    QObject::connect(&theFileManager, &FileManager::signalFileModified, &theDBManager, &DBManager::slotFileModified);
    QObject::connect(&theFileManager, &FileManager::signalFilesDeleted, &theDBManager, &DBManager::slotFilesDeleted);
    QObject::connect(&theFileManager, &FileManager::signalNewWorkingDirectory, &theDBManager, &DBManager::slotCreateNewDB);

    // qmlRegisterSingletonInstance("WikiClient.DataBackend", 1, 0, "FileManager", &theFileManager);
    // qmlRegisterSingletonInstance("WikiClient.DataBackend", 1, 0, "DBManager", &theDBManager);
    qmlRegisterType<LinkSuggestionProvider>("WikiClient.Suggestions", 1, 0, "LinkSuggestionProvider");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/views/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    QObject* root = engine.rootObjects().value(0);



    QObject::connect(root, SIGNAL(signalSetDirectory(QString)), &theFileManager, SLOT(slotSetDirectory(QString)));
    QObject::connect(root, SIGNAL(signalScanDirectory()), &theFileManager, SLOT(slotScanDirectory()));

    return app.exec();
}
