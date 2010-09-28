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

#ifndef STATISTICSDIALOG_H
#define STATISTICSDIALOG_H

#include <QDialog>

class QLabel;
class QVBoxLayout;
class QHBoxLayout;
class QPushButton;

class StatisticsDialog : public QDialog
{
    Q_OBJECT

public:
    StatisticsDialog(QWidget *parent = 0);

private slots:
    void updateContent();
    void resetClicked();

private:
    QLabel *label_;
    QPushButton *resetButton_;
    QPushButton *doneButton_;
    QVBoxLayout *mainLayout_;
    QHBoxLayout *buttonLayout_;
};

#endif
