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

#ifndef GAMEVIEW_H
#define GAMEVIEW_H

#include <QGraphicsView>

class GraphicsScene;
class PuzzleItem;

class GameView : public QGraphicsView
{
    Q_OBJECT

public:
    static GameView *instance();
    QList<PuzzleItem *> pieces() const;
    QPointF emptyPlace();
    void setEmptyPlace(const QPointF &place);
    bool areAllPiecesOk() const;

public slots:
    void setPieces(const QList<PuzzleItem *> pieces);
    void shufflePieces();

private:
    GameView(QWidget *parent = 0);

    static GameView *instance_;
    QGraphicsScene *scene_;
    QList<PuzzleItem *> pieces_;
    QPointF emptyPlace_;
    int hiddenIndex_;
};
#endif
