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

#include "statistics.h"
#include "defines.h"

#include <QDir>
#include <QFile>
#include <QTextStream>
#include <QStringList>

#include <QDebug>

Statistics *Statistics::instance_ = 0;

Statistics::Statistics(QObject *parent) :
        QObject(parent)
{
    moves_ << 0 << 0;
    minMoves_ << 0 << 0;
    maxMoves_ << 0 << 0;
    games_ << 0 << 0;

    readFile();
}

Statistics *Statistics::instance()
{
    if(!instance_) {
        instance_ = new Statistics;
    }

    return instance_;
}

int Statistics::gameCount(Difficulty difficulty) const
{
    return games_.at(difficulty);
}

int Statistics::totalGameCount() const
{
    int count = 0;
    for(int i = 0; i < games_.count(); ++i) {
        count += games_.at(i);
    }

    return count;
}

qreal Statistics::averageMoves(Difficulty difficulty) const
{
    qreal count = 0.0;
    if(games_.at(difficulty) > 0) {
        count = (moves_.at(difficulty) / games_.at(difficulty));
    }

    return count;
}

int Statistics::minMoves(Difficulty difficulty) const
{
    return minMoves_.at(difficulty);
}

int Statistics::maxMoves(Difficulty difficulty) const
{
    return maxMoves_.at(difficulty);
}

void Statistics::addNewScore(int moves, Difficulty difficulty)
{
    if(moves_.count() >= difficulty) {
        moves_[difficulty] += moves;
    }

    if(maxMoves_.count() >= difficulty) {
        if(maxMoves_.at(difficulty) < moves || maxMoves_.at(difficulty) == 0) {
            maxMoves_[difficulty] = moves;
        }
    }

    if(minMoves_.count() >= difficulty) {
        if(minMoves_.at(difficulty) > moves || minMoves_.at(difficulty) == 0) {
            minMoves_[difficulty] = moves;
        }
    }

    saveFile();
}

void Statistics::increaseGameCount(Difficulty difficulty)
{
    if(games_.count() >= difficulty) {
        games_[difficulty]++;
    }
}

void Statistics::readFile()
{
    QFile file(QString("%1/%2/%3")
               .arg(QDir::homePath())
               .arg(HOME_DIRECTORY)
               .arg(STATS_FILE));

    if(!file.exists()) {
        qDebug() << __PRETTY_FUNCTION__ << "No settings file";
        return;
    }

    if(!file.open(QIODevice::ReadOnly)) {
        return;
    }

    QTextStream in(&file);

    while(!in.atEnd()) {
        QString str = in.readLine();

        QStringList list = str.split(" ");

        if(list.count() == 5) {
            moves_[list.at(0).toInt()] = list.at(1).toInt();
            minMoves_[list.at(0).toInt()] = list.at(2).toInt();
            maxMoves_[list.at(0).toInt()] = list.at(3).toInt();
            games_[list.at(0).toInt()] = list.at(4).toInt();
        }
    }

    file.close();
}

void Statistics::saveFile()
{
    QDir dir(QString("%1/%2")
             .arg(QDir::homePath())
             .arg(HOME_DIRECTORY));
    if(!dir.exists()) {
        dir.mkpath(QString("%1/%2")
                   .arg(QDir::homePath())
                   .arg(HOME_DIRECTORY));
    }

    QFile file(QString("%1/%2/%3")
               .arg(QDir::homePath())
               .arg(HOME_DIRECTORY)
               .arg(STATS_FILE));

    if(!file.open(QIODevice::WriteOnly)) {
        return;
    }

    QTextStream out(&file);

    for(int i = 0; i < games_.count(); i++) {
        out << QString("%1 %2 %3 %4 %5\n")
                .arg(i)
                .arg(moves_.at(i))
                .arg(minMoves_.at(i))
                .arg(maxMoves_.at(i))
                .arg(games_.at(i));
    }

    file.close();
}

void Statistics::resetStatistics()
{
    moves_.clear();
    minMoves_.clear();
    maxMoves_.clear();
    games_.clear();

    moves_ << 0 << 0;
    minMoves_ << 0 << 0;
    maxMoves_ << 0 << 0;
    games_ << 0 << 0;

    saveFile();
}
