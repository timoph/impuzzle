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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class SettingsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    static MainWindow *instance();

public slots:
    void newGameClicked();

private slots:
    void importClicked();
    void aboutClicked();
    void gameEnded();
    void enableSaving();
    void showStatistics();

protected:
    void closeEvent(QCloseEvent *event);

private:
    MainWindow(QWidget *parent = 0);
    void createActions();
    void createMenu();

    static MainWindow *instance_;
    QAction *newGameAction_;
    QAction *importAction_;
    QAction *aboutAction_;
    QAction *saveAction_;
    QAction *statisticsAction_;
    QAction *quitAction_;

    QMenu *menu_;
    QMenu *helpMenu_;

    SettingsDialog *settingsDialog_;
};
#endif
