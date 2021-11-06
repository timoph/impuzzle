TEMPLATE = app
TARGET = impuzzle2
DEPENDPATH += .
INCLUDEPATH += .

VERSION = "2.0.0"
DEFINES+=APP_VERSION=\\\"$$VERSION\\\"

QT += quick

equals(QT_MAJOR_VERSION, 6) {
    QT += quickcontrols2
}

TRANSLATIONS = languages/impuzzle2_fi.ts \
               languages/impuzzle2_en.ts \
               languages/impuzzle2_sv.ts

bigscreen {
    DEFINES+=WINDOW_WIDTH=1280
    DEFINES+=WINDOW_HEIGHT=800
} else {
    DEFINES+=WINDOW_WIDTH=800
    DEFINES+=WINDOW_HEIGHT=480
}

HEADERS += \
    engine.h \
    imageimporter.h \
    puzzleitem.h \
    defines.h

SOURCES += \
    engine.cpp \
    main.cpp \
    imageimporter.cpp \
    puzzleitem.cpp

RESOURCES += resources.qrc \
    qml.qrc \
    translantions.qrc
