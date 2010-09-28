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

#ifndef STATISTICS_H
#define STATISTICS_H

#include <QObject>
#include <QList>

class Statistics : public QObject
{
    Q_OBJECT

public:
    enum Difficulty {
        easyDifficulty = 0,
        hardDifficulty
    };

    static Statistics *instance();

    int gameCount(Difficulty difficulty) const;

    int totalGameCount() const;

    qreal averageMoves(Difficulty difficulty) const;

    int minMoves(Difficulty difficulty) const;

    int maxMoves(Difficulty difficulty) const;

public slots:
    void addNewScore(int moves, Difficulty difficulty);

    void increaseGameCount(Difficulty difficulty);

    void readFile();

    void saveFile();

    void resetStatistics();

private:
    Statistics(QObject *parent = 0);

    static Statistics *instance_;

    QList<int> moves_;
    QList<int> minMoves_;
    QList<int> maxMoves_;
    QList<int> games_;
};

#endif
