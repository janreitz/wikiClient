#include "linkprovider.h"
#include "titlesuggestionprovider.h"
#include "sqltablemodelprovider.h"
#include "filemanager.h"
#include "dbmanager.h"
#include "editorbackend.h"
#include "settings.h"
#include "searchbackend.h"
#include "network.h"
#include "gitmanager.h"

#include "qgitglobal.h"

#include <QObject>
#include <QQuickItem>
#include <QQmlContext>
#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFontDatabase>
#include <QtWebView/QtWebView>


int main(int argc, char *argv[])
{
    LibQGit2::initLibQGit2();

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QtWebView::initialize();
    QGuiApplication app(argc, argv);

    app.setOrganizationName("Jan Reitz");
    app.setOrganizationDomain("JFF");
    app.setWindowIcon(QIcon(":/resources/icons/owl_scribbles_reduced.svg"));

    auto id = QFontDatabase::addApplicationFont(":/resources/fonts/FiraCode-Regular.ttf");
    QString family = QFontDatabase::applicationFontFamilies(id).at(0);

    qmlRegisterType<EditorBackend>("Backend", 1, 0, "EditorBackend");
    qmlRegisterType<Network>("Backend", 1, 0, "Network");
    qmlRegisterType<Node>("Backend", 1, 0, "Node");
    qmlRegisterType<Edge>("Backend", 1, 0, "Edge");

    Settings* settings = Settings::getInstance();
    settings->readSettingsFile(":/resources/settings/default.json");

    FileManager theFileManager;
    EditorBackend theEditorBackend;
    TitleSuggestionProvider theTitleSuggestionProvider;
    LinkProvider theLinkProvider;
    SqlTableModelProvider theTableModelProvider;
    SearchBackend theSearchBackend;

    QObject::connect(&theFileManager, &FileManager::signalNewFiles, DBManager::getInstance(), &DBManager::slotNewFiles);
    QObject::connect(&theFileManager, &FileManager::fileRenamed, DBManager::getInstance(), &DBManager::slotFileRenamed);
    QObject::connect(&theFileManager, &FileManager::signalFileModified, DBManager::getInstance(), &DBManager::slotFileModified);
    QObject::connect(&theFileManager, &FileManager::signalFilesDeleted, DBManager::getInstance(), &DBManager::slotFilesDeleted);
    QObject::connect(&theFileManager, &QFileSystemModel::rootPathChanged, DBManager::getInstance(), &DBManager::slotWorkingDirectoryChanged);
    QObject::connect(&theFileManager, &QFileSystemModel::rootPathChanged, GitManager::getInstance(), &GitManager::slotWorkingDirectoryChanged);
    QObject::connect(DBManager::getInstance(), &DBManager::signalDBAvailable, &theFileManager, &FileManager::slotScanDirectory);
    QObject::connect(DBManager::getInstance(), &DBManager::signalDBAvailable, &theTableModelProvider, &SqlTableModelProvider::slotDBOpened);


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
    engine.rootContext()->setContextProperty("theGitManager", GitManager::getInstance());
    engine.rootContext()->setContextProperty("theTitleSuggestionProvider", &theTitleSuggestionProvider);
    engine.rootContext()->setContextProperty("theLinkProvider", &theLinkProvider);
    engine.rootContext()->setContextProperty("theTableModelProvider", &theTableModelProvider);
    engine.rootContext()->setContextProperty("theSearchBackend", &theSearchBackend);

    engine.load(url);

    return app.exec();
}
