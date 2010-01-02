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
#include "introitem.h"

#include <QGraphicsScene>
#include <QDateTime>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFont>
#include <QMessageBox>

#include <QDebug>

GameView *GameView::instance_ = 0;

GameView::GameView(QWidget *parent) :
        QGraphicsView(parent)
{
    scene_ = new QGraphicsScene;
    hiddenIndex_ = -1;
    setScene(scene_);

    introItem_ = new IntroItem;
    introItem_->setText("Select new game from menu to play");
    scene_->addItem(introItem_);

    verticalStep_ = 0;
    horizontalStep_ = 0;

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
    horizontalStep_ = IMAGE_WIDTH / horizontalCount + 5;
    verticalStep_ = IMAGE_HEIGHT / verticalCount + 5;

    int pieceNumber = 0;

    // Set pieces to their correct positions
    for(int i = 0; i < verticalCount; ++i) {
        for(int j = 0; j < horizontalCount; ++j) {
            scene_->addItem(pieces_.at(pieceNumber));
            QPointF point(j * horizontalStep_, i * verticalStep_);
            pieces_.at(pieceNumber)->setPos(point);
            pieces_.at(pieceNumber)->setCorrectPlace(point);
            pieces_.at(pieceNumber)->setCurrentPlace(point);
            pieceNumber++;
        }
    }

    // Wait a second
    QTimer::singleShot(750, this, SLOT(shufflePieces()));
}

void GameView::shufflePieces()
{
    if(pieces_.isEmpty()) {
        qDebug() << "Empty list @ GameView::shufflePieces";
        return;
    }

    // Give pieces ramdom locations
    int rounds = 5; //TODO
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
        // Skip hidden piece
        if(i == hiddenIndex_) {
            continue;
        }
        // Id piece is not in it's place
        else if(pieces_.at(i)->correctPlace() != pieces_.at(i)->currentPlace()) {
            return false;
        }
    }
    // Show hidden piece and move it to it's place
    pieces_.at(hiddenIndex_)->show();
    pieces_.at(hiddenIndex_)->moveMeTo(emptyPlace_);

    // Set all pieces not movable
    for(int i = 0; i < pieces_.count(); ++i) {
        pieces_.at(i)->setMovable(false);
    }

    // Show dialog with move count
    QMessageBox::about(const_cast<GameView *>(this), tr("You won"), QString("Puzzle completed with %1 moves").arg(PuzzleItem::moveCount()));

    return true;
}

void GameView::setMovingPieces()
{
    if(pieces_.isEmpty()) {
        qDebug() << "Empty list @ GameView::setMovingPieces";
        return;
    }

    QPointF point = QPointF();
    for(int i = 0; i < pieces_.count(); ++i) {
        point = pieces_.at(i)->currentPlace();

        // Is piece on the left side of the empty space
        if(emptyPlace_.y() == point.y() && point.x() + horizontalStep_ == emptyPlace_.x()) {
            pieces_.at(i)->setMovable(true);
        }

        // Is piece on the right side of the empty space
        else if(emptyPlace_.y() == point.y() && point.x() - horizontalStep_ == emptyPlace_.x()) {
            pieces_.at(i)->setMovable(true);
        }

        // Is piece below the empty space
        else if(emptyPlace_.x() == point.x() && point.y() - verticalStep_ == emptyPlace_.y()) {
            pieces_.at(i)->setMovable(true);
        }

        // Is piece on top of the empty space
        else if(emptyPlace_.x() == point.x() && point.y() + verticalStep_ == emptyPlace_.y()) {
            pieces_.at(i)->setMovable(true);
        }

        // The piece is somewhere else
        else {
            pieces_.at(i)->setMovable(false);
        }
    }
}
