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

#include "statisticsdialog.h"
#include "statistics.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QPushButton>

StatisticsDialog::StatisticsDialog(QWidget *parent) :
        QDialog(parent)
{
    setModal(true);
    setWindowTitle(tr("Statistics"));

    resetButton_ = new QPushButton(tr("Reset"));
    doneButton_ = new QPushButton(tr("Close"));

    buttonLayout_ = new QHBoxLayout;
    buttonLayout_->addStretch();
    buttonLayout_->addWidget(resetButton_);
    buttonLayout_->addWidget(doneButton_);

    label_ = new QLabel;

    updateContent();

    mainLayout_ = new QVBoxLayout;
    mainLayout_->addWidget(label_);
    mainLayout_->addLayout(buttonLayout_);

    setLayout(mainLayout_);

    connect(resetButton_, SIGNAL(clicked()), this, SLOT(resetClicked()));
    connect(doneButton_, SIGNAL(clicked()), this, SLOT(close()));
}

void StatisticsDialog::updateContent()
{
    QString txt = QString("\tEasy\tHard\nGames\t%1\t%5\nBest\t%2\t%6\nAvg\t%3\t%7\nWorst\t%4\t%8\n")
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::easyDifficulty)))
    .arg(QString::number(Statistics::instance()->gameCount(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->minMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->averageMoves(Statistics::hardDifficulty)))
    .arg(QString::number(Statistics::instance()->maxMoves(Statistics::hardDifficulty)));

    label_->setText(txt);
}

void StatisticsDialog::resetClicked()
{
    Statistics::instance()->resetStatistics();
    updateContent();
}
