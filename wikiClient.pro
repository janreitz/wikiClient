QT += quick \
        sql \
        widgets \

CONFIG += c++17

LIBS += "C:/Users/jan-r/projects/libqgit2/build/bin/Debug/qgit2.lib"
INCLUDEPATH += "C:/Users/jan-r/projects/libqgit2/src"

LIBS += "C:/Users/jan-r/projects/libqgit2/build/libgit2/Debug/git2.lib"
INCLUDEPATH += "C:/Users/jan-r/projects/libgit2/include"

LIBS += "C:/Users/jan-r/projects/PerformanceSuite/target/release/PerformanceSuite.lib"
INCLUDEPATH += C:/Users/jan-r/projects/PerformanceSuite/source

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/abstractdbclient.cpp \
        src/dbmanager.cpp \
        src/document.cpp \
        src/edge.cpp \
        src/editorbackend.cpp \
        src/filemanager.cpp \
        src/fileparser.cpp \
        src/gitmanager.cpp \
        src/linkprovider.cpp \
        src/main.cpp \
        src/markdownhighlighter.cpp \
        src/network.cpp \
        src/node.cpp \
        src/qownlanguagedata.cpp \
        src/searchbackend.cpp \
        src/searchresult.cpp \
        src/settings.cpp \
        src/sqltablemodelprovider.cpp \
        src/suggestion.cpp \
        src/titlesuggestionprovider.cpp \
        src/utilities.cpp

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = qml/components

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/abstractdbclient.h \
    #src/bihash.h \
    #src/bihash.hpp \
    src/dbmanager.h \
    src/document.h \
    src/edge.h \
    src/editorbackend.h \
    src/filemanager.h \
    src/fileparser.h \
    src/gitmanager.h \
    src/linkprovider.h \
    src/markdownhighlighter.h \
    src/network.h \
    src/node.h \
    src/qownlanguagedata.h \
    src/searchbackend.h \
    src/searchresult.h \
    src/settings.h \
    src/sqltablemodelprovider.h \
    src/suggestion.h \
    src/titlesuggestionprovider.h \
    src/utilities.h

DISTFILES += \
    qml/components/MyColumnLayout.qml \
    qml/components/MyRowLayout.qml \
    qml/components/SideBar.qml \
    qml/js/TilingManager.js \
    qml/views/NetworkView.qml

