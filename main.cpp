/*
 ImPuzzle 2 - a 15-puzzle inspired game
 Copyright (C) 2023 Timo Rautiainen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QDir>
#include <QFile>
#include <iostream>
#include <QTranslator>
#include <QLoggingCategory>

#if QT_VERSION_MAJOR == 6
#include <QQuickStyle>
#endif

#include "puzzleitem.h"
#include "engine.h"

int main(int argc, char *argv[])
{
#if QT_VERSION_MAJOR == 6
    QQuickStyle::setStyle("Basic");
#else
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    // Mute the deprecated qml connect warnings.
    // Will be using the old syntax until this drops support for Qt 5.12
    QLoggingCategory::setFilterRules("qt.qml.connections.warning=false");

    QFont font("Helvetica");
    font.setPointSize(20);
    QGuiApplication::setFont(font);

    QGuiApplication app(argc, argv);
    app.setOrganizationName("timoph");
    app.setOrganizationDomain("timoph.fi");
    app.setApplicationName("ImPuzzle 2");
    app.setApplicationVersion(QString(APP_VERSION));

    QCommandLineParser cmdParser;
    QString appDesc = QString("%1%2%3%4%5").arg("ImPuzzle 2  Copyright (C) 2023  Timo Rautiainen\n")
            .arg("This program comes with ABSOLUTELY NO WARRANTY\n")
            .arg("This is free software, and you are welcome\n")
            .arg("to redistribute it under certain conditions\n")
            .arg("See https://www.gnu.org/licenses/gpl-3.0.txt for details.");
    cmdParser.setApplicationDescription(appDesc);

    QCommandLineOption shuffleOption(QStringList() << "s" << "shuffles",
                                    QCoreApplication::tr("Define how many moves are made to shuffle the pieces."),
                                     QCoreApplication::tr("count"));
    QCommandLineOption pathOption(QStringList() << "d" << "directory",
                                    QCoreApplication::tr("Define path where to look for images."),
                                  QCoreApplication::tr("directory path"));
    QCommandLineOption imageOption(QStringList() << "i" << "image",
                                    QCoreApplication::tr("Define image used to create the puzzle."),
                                   QCoreApplication::tr("file path"));
    QCommandLineOption languageOption(QStringList() << "l" << "language",
                                    QCoreApplication::tr("Define language."),
                                   QCoreApplication::tr("en|fi|sv"));
    QCommandLineOption fullScreenOption(QStringList() << "f" << "fullscreen",
                                    QCoreApplication::tr("Start in fullscreen."));
    QCommandLineOption moreFireworksOption(QStringList() << "m" << "morefireworks",
                                    QCoreApplication::tr("Show more fireworks after completing the puzzle."));
    QCommandLineOption helpOption(QStringList() << "h" << "help",
                                    QCoreApplication::tr("Show help."));
    cmdParser.addOption(shuffleOption);
    cmdParser.addOption(pathOption);
    cmdParser.addOption(imageOption);
    cmdParser.addOption(languageOption);
    cmdParser.addOption(fullScreenOption);
    cmdParser.addOption(moreFireworksOption);
    cmdParser.addOption(helpOption);

    cmdParser.process(app.arguments());

    QTranslator translator;
    QString lang = cmdParser.value(languageOption);
    bool success = false;
    if(lang == "fi") success = translator.load(":/languages/impuzzle2_fi.dm");
    else if(lang == "en") success = translator.load(":/languages/impuzzle2_en.dm");
    else if(lang == "sv") success = translator.load(":/languages/impuzzle2_sv.dm");
    else success = translator.load(":/languages/impuzzle2_en.dm");

    if(!success) qDebug() << "Failed to load translation";

    app.installTranslator(&translator);

    if(cmdParser.isSet(helpOption)) {
        std::cout << cmdParser.helpText().toStdString();
        return 0;
    }

    Engine::instance()->setShuffleCount(cmdParser.value(shuffleOption).toInt());
    QString searchPath = cmdParser.value(pathOption);
    QDir dir(searchPath);
    if(dir.exists()) Engine::instance()->setSearchPath(searchPath);
    QString userImage = cmdParser.value(imageOption);
    if(userImage != "" && QFile::exists(userImage)) Engine::instance()->setUserImageFile(userImage);
    bool fullScreen = cmdParser.isSet(fullScreenOption);
    bool moreFireworks = cmdParser.isSet(moreFireworksOption);

    qmlRegisterType<PuzzleItem>("impuzzle", 1, 0, "PuzzleItem");

    QQmlApplicationEngine engine;

    engine.rootContext()->setContextProperty("gameEngine", Engine::instance());
    engine.rootContext()->setContextProperty("windowWidth", QVariant(WINDOW_WIDTH));
    engine.rootContext()->setContextProperty("windowHeight", QVariant(WINDOW_HEIGHT));
    engine.rootContext()->setContextProperty("fullScreen", QVariant(fullScreen));
    engine.rootContext()->setContextProperty("showMoreFireWorks", QVariant(moreFireworks));

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
