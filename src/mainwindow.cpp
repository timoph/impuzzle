/*
  Image Puzzle - A set your pieces straight game
  Copyright (C) 2009  Timo Härkönen

  This program is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "mainwindow.h"
#include "gameview.h"
#include "settings.h"
#include "settingsdialog.h"
#include "aboutdialog.h"
#include "puzzleitem.h"
#include "statistics.h"
#include "statisticsdialog.h"

#include <QAction>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QCloseEvent>
#include <QApplication>

#include <QDebug>

#include "imageimporter.h"

MainWindow *MainWindow::instance_ = 0;

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    createActions();
    createMenu();

    setCentralWidget(GameView::instance());
    settingsDialog_ = new SettingsDialog(this);

    setWindowTitle(tr("ImPuzzle"));

    connect(GameView::instance(), SIGNAL(gameWon()), this, SLOT(gameEnded()));
    connect(GameView::instance(), SIGNAL(gameRestored()), this, SLOT(enableSaving()));
}

MainWindow *MainWindow::instance()
{
    if(!instance_) {
        instance_ = new MainWindow;
    }

    return instance_;
}

void MainWindow::createMenu()
{
    menu_ = menuBar()->addMenu("&Game");
    menu_->addAction(newGameAction_);
    menu_->addAction(statisticsAction_);
    menu_->addAction(saveAction_);
    menu_->addAction(quitAction_);
    //menu_->addAction(importAction_);

    helpMenu_ = menuBar()->addMenu("&Help");
    helpMenu_->addAction(aboutAction_);
}

void MainWindow::createActions()
{
    newGameAction_ = new QAction(tr("New game"), this);
    connect(newGameAction_, SIGNAL(triggered()), this, SLOT(newGameClicked()));

    importAction_ = new QAction(tr("Import image"), this);
    connect(importAction_, SIGNAL(triggered()), this, SLOT(importClicked()));
    importAction_->setDisabled(true);

    aboutAction_ = new QAction(tr("About ImPuzzle"), this);
    connect(aboutAction_, SIGNAL(triggered()), this, SLOT(aboutClicked()));

    saveAction_ = new QAction(tr("Save and quit"), this);
    connect(saveAction_, SIGNAL(triggered()), GameView::instance(), SLOT(saveGame()));
    saveAction_->setDisabled(true);

    statisticsAction_ = new QAction(tr("Statistics"), this);
    connect(statisticsAction_, SIGNAL(triggered()), this, SLOT(showStatistics()));

    quitAction_ = new QAction(tr("Quit"), this);
    connect(quitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));
}

void MainWindow::importClicked()
{

}

void MainWindow::newGameClicked()
{
    int r = settingsDialog_->exec();

    if(r) {
        GameView::instance()->setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()));
        enableSaving();
        PuzzleItem::setMoveCount(0);
    }
}

void MainWindow::aboutClicked()
{
    AboutDialog *dialog = new AboutDialog(this);
    dialog->exec();
    dialog->deleteLater();
}

void MainWindow::gameEnded()
{
    if(saveAction_->isEnabled()) {
        saveAction_->setDisabled(true);
        Statistics::instance()->increaseGameCount(Settings::instance()->pieceCount() == EASY_PIECE_COUNT ? Statistics::easyDifficulty : Statistics::hardDifficulty);
        Statistics::instance()->addNewScore(PuzzleItem::moveCount(),
                                            Settings::instance()->pieceCount() == EASY_PIECE_COUNT ? Statistics::easyDifficulty : Statistics::hardDifficulty);
        PuzzleItem::resetMoveCount();
    }
}

void MainWindow::enableSaving()
{
    if(!saveAction_->isEnabled()) {
        saveAction_->setEnabled(true);
    }
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if(saveAction_->isEnabled()) {
        int answer = QMessageBox::question(this, tr("Save game status?"),
                                           tr("Saved status will be automatically loaded when you start the application next time"),
                                           QMessageBox::Yes, QMessageBox::No);

        if(answer == QMessageBox::Yes) {
            GameView::instance()->saveGame();
        }
    }

    event->accept();
}

void MainWindow::showStatistics()
{
    StatisticsDialog dialog(this);
    dialog.exec();
}
