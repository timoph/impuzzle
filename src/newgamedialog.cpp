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

#include "newgamedialog.h"

#include <QComboBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QStringList>

NewGameDialog::NewGameDialog(QWidget *parent) :
        QDialog(parent)
{
    imageLabel_ = new QLabel(tr("Select image"));
    imageCombo_ = new QComboBox;

    piecesLabel_ = new QLabel(tr("Set piece count"));
    piecesCombo_ = new QComboBox;

    QStringList pieceList;
    pieceList << "12" << "20" << "30";
    piecesCombo_->addItems(pieceList);

    comboLayout_ = new QHBoxLayout;
    comboLayout_->addWidget(imageLabel_);
    comboLayout_->addWidget(imageCombo_);
    comboLayout_->addStretch();

    startButton_ = new QPushButton(tr("Start"));

    buttonLayout_ = new QHBoxLayout;
    buttonLayout_->addWidget(piecesLabel_);
    buttonLayout_->addWidget(piecesCombo_);
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(startButton_);

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addLayout(comboLayout_);
    mainLayout_->addLayout(buttonLayout_);

    setLayout(mainLayout_);

    //connect(startButton_, SIGNAL(clicked()))
}
