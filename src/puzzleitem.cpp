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

#include "puzzleitem.h"
#include "gameview.h"

#include <QGraphicsSceneMouseEvent>
#include <QPropertyAnimation>
#include <QPainter>
#include <QFont>
#include <QFontMetrics>

int PuzzleItem::moveCount_ = 0;

PuzzleItem::PuzzleItem(QGraphicsItem *parent) :
        QGraphicsPixmapItem(parent)
{
    movable_ = true;
    moveAnimation_ = new QPropertyAnimation(this, "pos", this);
    pieceNumber_ = 0;
    drawNumber_ = true;
}

QPointF PuzzleItem::correctPlace() const
{
    return correctPlace_;
}

QPointF PuzzleItem::currentPlace() const
{
    return currentPlace_;
}

void PuzzleItem::setCorrectPlace(const QPointF &place)
{
    correctPlace_ = place;
}

void PuzzleItem::setCurrentPlace(const QPointF &place)
{
    currentPlace_ = place;
}

bool PuzzleItem::movable() const
{
    return movable_;
}

void PuzzleItem::setMovable(bool canMove)
{
    movable_ = canMove;
}

void PuzzleItem::moveMeTo(const QPointF &location)
{
    moveAnimation_->setStartValue(currentPlace());
    moveAnimation_->setEndValue(location);
    moveAnimation_->start();
}

void PuzzleItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(movable_) {
        moveMeTo(GameView::instance()->emptyPlace());
        QPointF tmp = currentPlace();
        setCurrentPlace(GameView::instance()->emptyPlace());
        GameView::instance()->setEmptyPlace(tmp);
        event->accept();

        moveCount_++;

        // If piece is in its place check if we won the game
        bool won = false;
        if(currentPlace() == correctPlace()) {
            won = GameView::instance()->areAllPiecesOk();
        }

        // if we didn't win set pieces that can be moved
        if(!won) {
            GameView::instance()->setMovingPieces();
        }
    }
    else {
        event->ignore();
    }
}

void PuzzleItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
}

int PuzzleItem::moveCount()
{
    return moveCount_;
}

void PuzzleItem::resetMoveCount()
{
    moveCount_ = 0;
}

void PuzzleItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QGraphicsPixmapItem::paint(painter, option, widget);

    if(drawNumber_) {
        painter->save();

        QFont font = painter->font();
        QFontMetrics metrics(font);
        QRect numberRect(0, 0, metrics.height(), metrics.height());

        painter->setPen(Qt::NoPen);

        painter->setBrush(QColor(255, 255, 255, 192));
        painter->drawRect(numberRect);

        painter->setPen(Qt::black);

        QTextOption textOption;
        textOption.setAlignment(Qt::AlignCenter);

        painter->drawText(numberRect, QString::number(pieceNumber_), textOption);

        painter->restore();
    }
}

int PuzzleItem::pieceNumber() const
{
    return pieceNumber_;
}

void PuzzleItem::setPieceNumber(const int pieceNumber)
{
    pieceNumber_ = pieceNumber;
}

void PuzzleItem::setDrawNumber(bool value)
{
    if(value != drawNumber_) {
        drawNumber_ = value;
        update();
    }
}

bool PuzzleItem::drawNumber() const
{
    return drawNumber_;
}

void PuzzleItem::setMoveCount(int count)
{
    moveCount_ = count;
}
