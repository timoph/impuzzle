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

#include <QAction>
#include <QMenu>
#include <QMenuBar>

#include "imageimporter.h"

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent)
{
    createActions();
    createMenu();

    setCentralWidget(GameView::instance());
}

void MainWindow::createMenu()
{
    menu_ = menuBar()->addMenu("");
    menu_->addAction(newGameAction_);
    menu_->addAction(importAction_);
}

void MainWindow::createActions()
{
    newGameAction_ = new QAction(tr("New game"), this);
    connect(newGameAction_, SIGNAL(triggered()), this, SLOT(newGameClicked()));

    importAction_ = new QAction(tr("Import image"), this);
    connect(importAction_, SIGNAL(triggered()), this, SLOT(importClicked()));
    importAction_->setDisabled(true);
}

void MainWindow::importClicked()
{

}

void MainWindow::newGameClicked()
{
    GameView::instance()->setPieces(ImageImporter::instance()->newPieces(0, 20));
}
