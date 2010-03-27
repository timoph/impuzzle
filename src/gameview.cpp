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
#include "imageimporter.h"
#include "settings.h"

#include <QGraphicsScene>
#include <QDateTime>
#include <QTimer>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QFont>
#include <QMessageBox>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QCloseEvent>

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

    verticalStep_ = 0;
    horizontalStep_ = 0;

    qsrand(QDateTime::currentDateTime().toTime_t());

    if(QFile::exists(QString("%1/%2/%3")
                    .arg(QDir::homePath()).arg(HOME_DIRECTORY).arg(RESTORE_FILE))) {
        if(!restoreGame()) {
            setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()));
        }
    }
    else {
        scene_->addItem(introItem_);
    }
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

void GameView::setPieces(const QList<PuzzleItem *> pieces, bool shuffle)
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
    if(pieces_.count() == EASY_PIECE_COUNT) {
        horizontalCount = EASY_HORIZONTAL_COUNT;
    }
    else if(pieces_.count() == HARD_PIECE_COUNT) {
        horizontalCount = HARD_HORIZONTAL_COUNT;
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
            pieces_.at(pieceNumber)->setDrawNumber(true);
            pieceNumber++;
        }
    }

    // Wait and shuffle if desired
    if(shuffle) {
        QTimer::singleShot(750, this, SLOT(shufflePieces()));
    }
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

    setMovingPieces();
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

    // Set all pieces not movable and hide numbers
    for(int i = 0; i < pieces_.count(); ++i) {
        pieces_.at(i)->setMovable(false);
        pieces_.at(i)->setDrawNumber(false);
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

bool GameView::restoreGame()
{
    // Read settings from file
    QFile file(QString("%1/%2/%3")
               .arg(QDir::homePath())
               .arg(HOME_DIRECTORY)
               .arg(RESTORE_FILE));

    if(!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Failed to open restore file for reading";
        return false;
    }

    QTextStream in(&file);

    QStringList list;

    list = in.readLine().split(";;");

    qDebug() << "restore list count: " << list.count();

    if(!list.isEmpty()) {
        Settings::instance()->setPieceCount(list.at(0).toInt());

        QString im = list.at(1);
        if(im == "default" || im.isEmpty()) {
            Settings::instance()->setImage(0);
            Settings::instance()->setImagePath("default");
        }
        else {
            Settings::instance()->setImagePath(im);
            Settings::instance()->setImage(QPixmap(im));
        }
        PuzzleItem::setMoveCount(list.at(2).toInt());

        setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()), false);

        qDebug() << "pieces_ count after restoring image: " << pieces_.count();

        if(list.count() >= pieces_.count() + 3) {
            for(int j = 0; j < pieces_.count(); ++j) {
                if(!list.at(j + 3).isNull()) {
                    QStringList points = list.at(j + 3).split("#");
                    //if(points.count() == 2)
                    QPointF point(points.at(0).toInt(), points.at(1).toInt());

                    qDebug() << "Setting piece " << pieces_.at(j)->pieceNumber();
                    qDebug() << "x: " << point.x() << " y: " << point.y();

                    pieces_.at(j)->setCurrentPlace(point);
                }
            }
        }
        else {
            file.close();
            file.remove();
            return false;
        }

        QStringList hidden = list.last().split("#");

        if(hidden.count() == 3) {
            for(int m = 0; m < pieces_.count(); ++m) {
                pieces_.at(m)->setPos(pieces_.at(m)->currentPlace());
                if(pieces_.at(m)->pieceNumber() == hidden.at(2).toInt()) {
                    qDebug() << "Hiding piece number " << hidden;
                    hiddenIndex_ = m;
                }
            }

            setEmptyPlace(QPointF(hidden.at(0).toInt(), hidden.at(1).toInt()));

            pieces_.at(hiddenIndex_)->setVisible(false);

            setMovingPieces();
        }
        else {
            // TODO: revert
            setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()));
            file.close();
            file.remove();
            return false;
        }
    }
    else {
        qDebug() << "Invalid restore file";
        file.close();
        file.remove();
        return false;
    }

    file.close();
    file.remove();

    return true;
}

bool GameView::saveGame()
{
    if(pieces_.isEmpty() || pieces_.count() < EASY_PIECE_COUNT) {
        return false;
    }

    QDir dir;
    if(!dir.exists(QString("%1/%2")
                    .arg(QDir::homePath())
                    .arg(HOME_DIRECTORY))) {
        dir.mkpath(QString("%1/%2")
                   .arg(QDir::homePath())
                   .arg(HOME_DIRECTORY));
    }

    QFile file(QString("%1/%2/%3")
               .arg(QDir::homePath())
               .arg(HOME_DIRECTORY)
               .arg(RESTORE_FILE));

    if(!file.open(QIODevice::WriteOnly)) {
        qDebug() << "Failed to open restore file for writing";
        return false;
    }

    QTextStream out(&file);

    out << Settings::instance()->pieceCount();
    out << ";;";
    if(Settings::instance()->imagePath().isEmpty()) {
        out << "default";
    }
    else {
        out << Settings::instance()->imagePath();
    }
    out << ";;";
    out << PuzzleItem::moveCount();
    out << ";;";

    // piece positions
    int number = 0;
    int hiddenNo = 0;

    while(number != pieces_.count()) {
        for(int i = 0; i < pieces_.count(); ++i) {
            if(pieces_.at(i)->pieceNumber() == number + 1) {
                out << pieces_.at(i)->currentPlace().x();
                out << "#";
                out << pieces_.at(i)->currentPlace().y();
                out << ";;";
                pieces_.at(i)->pieceNumber();
                if(!pieces_.at(i)->isVisible()) {
                    hiddenNo = number + 1;
                }
                number++;
                break;
            }
        }
    }

    out << QString("%1#%2#%3").arg(emptyPlace().x()).arg(emptyPlace().y()).arg(hiddenNo);

    out << "\n";

    file.close();

    return true;
}

void GameView::closeEvent(QCloseEvent *event)
{
    int answer = QMessageBox::question(this, tr("Save game status?"),
                                       tr("Saved status will be automatically loaded when you start the application next time"),
                                       QMessageBox::Yes, QMessageBox::No);

    if(answer == QMessageBox::Yes) {
        saveGame();
    }

    event->accept();
}
