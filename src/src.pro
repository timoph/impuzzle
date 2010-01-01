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
    defines.h
SOURCES += gameview.cpp \
    main.cpp \
    mainwindow.cpp \
    imageimporter.cpp \
    puzzleitem.cpp \
    newgamedialog.cpp
RESOURCES += resources.qrc
