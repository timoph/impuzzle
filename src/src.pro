TEMPLATE = app
TARGET = impuzzle
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../bin
maemo5:QT += maemo5

greaterThan(QT_MAJOR_VERSION, 4) {
	QT += core widgets
	DEFINES += QT5BUILD
}

macx {
	CONFIG -= app_bundle
}

# Input
HEADERS += gameview.h \
    mainwindow.h \
    imageimporter.h \
    puzzleitem.h \
    defines.h \
    introitem.h \
    settings.h \
    settingsdialog.h \
    aboutdialog.h \
    statistics.h \
    statisticsdialog.h \
    trackerfiles.h
SOURCES += gameview.cpp \
    main.cpp \
    mainwindow.cpp \
    imageimporter.cpp \
    puzzleitem.cpp \
    introitem.cpp \
    settings.cpp \
    settingsdialog.cpp \
    aboutdialog.cpp \
    statistics.cpp \
    statisticsdialog.cpp \
    trackerfiles.cpp
RESOURCES += resources.qrc

unix:!macx {
desktop.files += impuzzle.desktop
desktop.path = /usr/share/applications/hildon/
icon.files += images/impuzzle.png
icon.path = /usr/share/pixmaps/
target.path = /opt/impuzzle/
INSTALLS += target \
    desktop \
    icon
}
