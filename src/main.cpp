#include "linkprovider.h"
#include "titlesuggestionprovider.h"
#include "sqltablemodelprovider.h"
#include "filemanager.h"
#include "dbmanager.h"
#include "editorbackend.h"
#include "settings.h"
#include "searchbackend.h"
#include "network.h"

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
    app.setWindowIcon(QIcon(":/resources/icons/owl_scribbles_reduced.svg"));

    qmlRegisterType<EditorBackend>("Backend", 1, 0, "EditorBackend");
    qmlRegisterType<Node>("Backend", 1, 0, "Node");
    qmlRegisterType<Edge>("Backend", 1, 0, "Edge");

    Settings* settings = Settings::getInstance();
    settings->readSettingsFile(":/resources/settings/default.json");

    FileManager theFileManager;
    DBManager theDBManager;
    EditorBackend theEditorBackend;
    TitleSuggestionProvider theTitleSuggestionProvider(&theDBManager);
    LinkProvider theLinkProvider(&theDBManager);
    SqlTableModelProvider theTableModelProvider(&theDBManager);
    SearchBackend theSearchBackend(&theDBManager);
    Network theNetwork(&theDBManager);

    QObject::connect(&theFileManager, &FileManager::signalNewFiles, &theDBManager, &DBManager::slotNewFiles);
    QObject::connect(&theFileManager, &FileManager::fileRenamed, &theDBManager, &DBManager::slotFileRenamed);
    QObject::connect(&theFileManager, &FileManager::signalFileModified, &theDBManager, &DBManager::slotFileModified);
    QObject::connect(&theFileManager, &FileManager::signalFilesDeleted, &theDBManager, &DBManager::slotFilesDeleted);
    QObject::connect(&theFileManager, &QFileSystemModel::rootPathChanged, &theDBManager, &DBManager::slotWorkingDirectoryChanged);
    QObject::connect(&theDBManager, &DBManager::signalDBAvailable, &theFileManager, &FileManager::slotScanDirectory);
    QObject::connect(&theDBManager, &DBManager::signalDBAvailable, &theTableModelProvider, &SqlTableModelProvider::slotDBOpened);
    QObject::connect(&theDBManager, &DBManager::signalDBInitialized, &theNetwork, &Network::initializeNetwork);

    theFileManager.setRootPath(settings->rootDirectory());
    

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml/views/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);

    engine.rootContext()->setContextProperty("theSettings", settings);
    engine.rootContext()->setContextProperty("theFileManager", &theFileManager);
    engine.rootContext()->setContextProperty("theTitleSuggestionProvider", &theTitleSuggestionProvider);
    engine.rootContext()->setContextProperty("theLinkProvider", &theLinkProvider);
    engine.rootContext()->setContextProperty("theTableModelProvider", &theTableModelProvider);
    engine.rootContext()->setContextProperty("theSearchBackend", &theSearchBackend);
    engine.rootContext()->setContextProperty("theNetwork", &theNetwork);

    engine.load(url);



    return app.exec();
}
