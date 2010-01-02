TEMPLATE = app
TARGET = impuzzle
DEPENDPATH += .
INCLUDEPATH += .
DESTDIR = ../bin

# Input
HEADERS += gameview.h \
    mainwindow.h \
    imageimporter.h \
    puzzleitem.h \
    newgamedialog.h \
    defines.h \
    introitem.h \
    settings.h \
    settingsdialog.h

SOURCES += gameview.cpp \
    main.cpp \
    mainwindow.cpp \
    imageimporter.cpp \
    puzzleitem.cpp \
    newgamedialog.cpp \
    introitem.cpp \
    settings.cpp \
    settingsdialog.cpp

RESOURCES += resources.qrc

desktop.files += impuzzle.desktop
desktop.path = /usr/share/applications/hildon/

target.path = /opt/impuzzle/

INSTALLS += target desktop
