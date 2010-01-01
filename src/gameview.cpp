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

#include "gameview.h"
#include "puzzleitem.h"
#include "defines.h"

#include <QGraphicsScene>
#include <QDateTime>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFont>

#include <QDebug>

GameView *GameView::instance_ = 0;

GameView::GameView(QWidget *parent) :
        QGraphicsView(parent)
{
    scene_ = new QGraphicsScene;
    hiddenIndex_ = -1;
    setScene(scene_);

    qsrand(QDateTime::currentDateTime().toTime_t());
}

GameView *GameView::instance()
{
    if(!instance_) {
        instance_ = new GameView;
    }

    return instance_;
}

QList<PuzzleItem *> GameView::pieces() const
{
    return pieces_;
}

void GameView::setPieces(const QList<PuzzleItem *> pieces)
{
    if(pieces.isEmpty()) {
        qDebug() << "Empty list @ GameView::setPieces";
        return;
    }

    QList<QGraphicsItem *> previousItems = scene_->items();
    if(!previousItems.isEmpty()) {
        foreach(QGraphicsItem *item, previousItems) {
            scene_->removeItem(item);
        }
    }

    pieces_ = pieces;

    int horizontalCount = 0;

    // Find out board size
    if(pieces_.count() == 12) {
        horizontalCount = 4;
    }
    else if(pieces_.count() == 20) {
        horizontalCount = 5;
    }
    else {
        qDebug() << "Invalid piece count @ GameView::setPieces";
        qDebug() << QString("Count was %1").arg(pieces_.count());
        return;
    }

    int verticalCount = pieces_.count() / horizontalCount;
    int horizontalStep = IMAGE_WIDTH / horizontalCount + 5;
    int verticalStep = IMAGE_HEIGHT / verticalCount + 5;

    int pieceNumber = 0;

    // Set pieces to their correct positions
    for(int i = 0; i < verticalCount; ++i) {
        for(int j = 0; j < horizontalCount; ++j) {
            scene_->addItem(pieces_.at(pieceNumber));
            QPointF point(j * horizontalStep, i * verticalStep);
            pieces_.at(pieceNumber)->setPos(point);
            pieces_.at(pieceNumber)->setCorrectPlace(point);
            pieces_.at(pieceNumber)->setCurrentPlace(point);
            pieceNumber++;
        }
    }

    // Wait a second
    QTimer::singleShot(1000, this, SLOT(shufflePieces()));
}

void GameView::shufflePieces()
{
    if(pieces_.isEmpty()) {
        qDebug() << "Empty list @ GameView::shufflePieces";
        return;
    }

    // TODO Give pieces ramdom locations
    int rounds = 5;
    for(int j = 0; j < rounds; ++j) {
        for(int i = 0; i < pieces_.count(); ++i) {
            QPointF tmp;
            int changeIndex = 0;
            while(changeIndex == i) {
                changeIndex = qrand() % pieces_.count();
            }
            tmp = pieces_.at(changeIndex)->currentPlace();
            pieces_.at(changeIndex)->setCurrentPlace(pieces_.at(i)->currentPlace());
            pieces_.at(i)->setCurrentPlace(tmp);
        }
    }

    // TODO Animate transitions to new locations
    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    for(int i = 0; i < pieces_.count(); ++i) {
        QPropertyAnimation *animation = new QPropertyAnimation(pieces_.at(i), "pos");
        animation->setStartValue(pieces_.at(i)->correctPlace());
        animation->setEndValue(pieces_.at(i)->currentPlace());
        animation->setDuration(750);
        animation->setEasingCurve(QEasingCurve::InOutCirc);
        animationGroup->addAnimation(animation);
    }
    animationGroup->start();

    // Hide random piece
    int hiddenPiece = qrand() % pieces_.count();
    emptyPlace_ = pieces_.at(hiddenPiece)->currentPlace();
    pieces_.at(hiddenPiece)->hide();
    hiddenIndex_ = hiddenPiece;
}

QPointF GameView::emptyPlace()
{
    return emptyPlace_;
}

void GameView::setEmptyPlace(const QPointF &place)
{
    emptyPlace_ = place;
}

bool GameView::areAllPiecesOk() const
{
    for(int i = 0; i < pieces_.count(); ++i) {
        if(i == hiddenIndex_) {
            continue;
        }
        else if(pieces_.at(i)->correctPlace() != pieces_.at(i)->currentPlace()) {
            return false;
        }
    }
    pieces_.at(hiddenIndex_)->show();
    pieces_.at(hiddenIndex_)->moveMeTo(emptyPlace_);

    for(int i = 0; i < pieces_.count(); ++i) {
        pieces_.at(i)->setMovable(false);
    }

    return true;
}
