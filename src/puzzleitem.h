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

#ifndef PUZZLEITEM_H
#define PUZZLEITEM_H

#include <QGraphicsPixmapItem>
#include <QObject>

class QPropertyAnimation;

class PuzzleItem : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
    Q_PROPERTY(QPointF pos READ pos WRITE setPos)

public:
    PuzzleItem(QGraphicsItem *parent = 0);
    QPointF correctPlace() const;
    QPointF currentPlace() const;
    void setCorrectPlace(const QPointF &place);
    void setCurrentPlace(const QPointF &place);
    bool movable() const;
    void setMovable(bool canMove);
    void moveMeTo(const QPointF &location);
    static int moveCount();
    static void setMoveCount(const int count);
    static void resetMoveCount();
    static void setManuallyMovable(const bool manuallyMovable);
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    void setPieceNumber(const int pieceNumber);
    int pieceNumber() const;
    void setDrawNumber(bool value);
    bool drawNumber() const;

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QPointF correctPlace_;
    QPointF currentPlace_;
    bool movable_;
    QPropertyAnimation *moveAnimation_;
    static int moveCount_;
    static bool manuallyMovable_;
    int pieceNumber_;
    bool drawNumber_;
};
#endif
