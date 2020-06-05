#include "linkprovider.h"
#include "titlesuggestionprovider.h"
#include "filemanager.h"
#include "dbmanager.h"
#include "editorbackend.h"

#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    app.setOrganizationName("Jan Reitz");
    app.setOrganizationDomain("JFF");

    qmlRegisterType<EditorBackend>("Backend", 1, 0, "EditorBackend");

    FileManager theFileManager;
    DBManager theDBManager;
    EditorBackend theEditorBackend;
    TitleSuggestionProvider theTitleSuggestionProvider(&theDBManager);
    LinkProvider theLinkProvider(&theDBManager);

    QObject::connect(&theFileManager, &FileManager::signalNewFiles, &theDBManager, &DBManager::slotNewFiles);
    QObject::connect(&theFileManager, &FileManager::fileRenamed, &theDBManager, &DBManager::slotFileRenamed);
    QObject::connect(&theFileManager, &FileManager::signalFileModified, &theDBManager, &DBManager::slotFileModified);
    QObject::connect(&theFileManager, &FileManager::signalFilesDeleted, &theDBManager, &DBManager::slotFilesDeleted);
    QObject::connect(&theFileManager, &FileManager::rootPathChanged, &theDBManager, &DBManager::slotRootDirectoryChanged);

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/views/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);


    engine.rootContext()->setContextProperty("theFileManager", &theFileManager);
    //engine.rootContext()->setContextProperty("theEditorBackend", &theEditorBackend);
    engine.rootContext()->setContextProperty("theTitleSuggestionProvider", &theTitleSuggestionProvider);
    engine.rootContext()->setContextProperty("theLinkProvider", &theLinkProvider);

    return app.exec();
}
