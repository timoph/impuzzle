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
#include <QFileInfo>
#include <QDateTime>
#include <QTimer>
#include <QApplication>
#include <QDateTime>
#include <QDesktopWidget>
#ifdef QT5BUILD
#include <QTransform>
#endif

#ifdef Q_WS_MAEMO_5
#include <QMaemo5InformationBox>
#endif

#include <QDebug>

GameView *GameView::instance_ = 0;

GameView::GameView(QWidget *parent) :
        QGraphicsView(parent)
{
    setBackgroundBrush(Qt::black);
    qsrand(QDateTime::currentDateTime().toTime_t());
    scene_ = new QGraphicsScene;
    hiddenIndex_ = -1;
    setScene(scene_);

    introItem_ = new IntroItem;
    introItem_->setText("- ImPuzzle -");

    verticalStep_ = 0;
    horizontalStep_ = 0;

    qsrand(QDateTime::currentDateTime().toTime_t());

    if(QFile::exists(QString("%1/%2/%3")
                    .arg(QDir::homePath()).arg(HOME_DIRECTORY).arg(RESTORE_FILE))) {
        QDesktopWidget dw;
        QRect rect = dw.availableGeometry();
        Settings::setImageWidth(rect.width() * 0.8);
        Settings::setImageHeight(rect.height() * 0.8);
        if(!restoreGame()) {
            setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()));
            PuzzleItem::setMoveCount(0);
        }
        else {
            QTimer::singleShot(0, this, SIGNAL(gameRestored()));
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
    PuzzleItem::setManuallyMovable(false);

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
    horizontalStep_ = Settings::imageWidth() / horizontalCount + 5;
    verticalStep_ = Settings::imageHeight() / verticalCount + 5;

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
    else {
        PuzzleItem::setManuallyMovable(true);
    }
}

//TODO: fixme!
void GameView::shufflePieces()
{
    if(pieces_.isEmpty()) {
        qDebug() << "Empty list @ GameView::shufflePieces";
        return;
    }

    // Give pieces ramdom locations
    hiddenIndex_ = qrand() % pieces_.count();
    emptyPlace_ = pieces_.at(hiddenIndex_)->currentPlace();

    QPointF topLeft = pieces_.at(0)->correctPlace();
    QPointF bottomRight = pieces_.last()->correctPlace();

    int moveCount = pieces_.count() * 10;
    int movesMade = 0;

    PuzzleItem *item = 0;

    for(int i = 0; i < moveCount; ++i) {
        int rand = qrand() % 4;

        switch(rand) {
        // up
        case 0:
            if(pieces_.at(hiddenIndex_)->currentPlace().y() > topLeft.y()) {
                QPointF tmp = pieces_.at(hiddenIndex_)->currentPlace();
#ifdef QT5BUILD
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(0, -verticalStep_), QTransform());
#else
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(0, -verticalStep_));
#endif
                if(graphicsItem) {
                    item = dynamic_cast<PuzzleItem *>(graphicsItem);
                    if(item->movable()) {
                        emptyPlace_ = item->currentPlace();
                        pieces_.at(hiddenIndex_)->setCurrentPlace(item->currentPlace());
                        pieces_.at(hiddenIndex_)->setPos(item->currentPlace());
                        item->setCurrentPlace(tmp);
                        item->setPos(tmp);
                        invalidateScene();
                        scene()->update();
                        setMovingPieces();
                        movesMade++;
                    }
                    else {
                        qDebug() << "Item right of hidden piece not movable";
                    }
                }
            }
            else {
                --i;
            }
            break;
        // down
        case 1:
            if(pieces_.at(hiddenIndex_)->currentPlace().y() < bottomRight.y()) {
                QPointF tmp = pieces_.at(hiddenIndex_)->currentPlace();
#ifdef QT5BUILD
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(0, verticalStep_), QTransform());
#else
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(0, verticalStep_));
#endif
				if(graphicsItem) {
                    item = dynamic_cast<PuzzleItem *>(graphicsItem);
                    if(item->movable()) {
                    emptyPlace_ = item->currentPlace();
                        pieces_.at(hiddenIndex_)->setCurrentPlace(item->currentPlace());
                        pieces_.at(hiddenIndex_)->setPos(item->currentPlace());
                        item->setCurrentPlace(tmp);
                        item->setPos(tmp);
                        setMovingPieces();
                        movesMade++;
                    }
                    else {
                        qDebug() << "Item down of hidden piece not movable";
                    }
                }
            }
            else {
                --i;
            }
            break;
        // left
        case 2:
            if(pieces_.at(hiddenIndex_)->currentPlace().x() > topLeft.x()) {
                QPointF tmp = pieces_.at(hiddenIndex_)->currentPlace();
#ifdef QT5BUILD
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(-horizontalStep_, 0), QTransform());
#else
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(-horizontalStep_, 0));
#endif
				if(graphicsItem) {
                    item = dynamic_cast<PuzzleItem *>(graphicsItem);
                    if(item->movable()) {
                        emptyPlace_ = item->currentPlace();
                        pieces_.at(hiddenIndex_)->setCurrentPlace(item->currentPlace());
                        pieces_.at(hiddenIndex_)->setPos(item->currentPlace());
                        item->setCurrentPlace(tmp);
                        item->setPos(tmp);
                        setMovingPieces();
                        movesMade++;
                    }
                    else {
                        qDebug() << "Item left of hidden piece not movable";
                    }
                }
            }
            else {
                --i;
            }
            break;
        // right
        case 3:
            if(pieces_.at(hiddenIndex_)->currentPlace().x() < bottomRight.x()) {
                QPointF tmp = pieces_.at(hiddenIndex_)->currentPlace();
#ifdef QT5BUILD
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(horizontalStep_, 0), QTransform());
#else
                QGraphicsItem *graphicsItem = scene()->itemAt(tmp + QPointF(horizontalStep_, 0), QTransform());
#endif
				if(graphicsItem) {
                    item = dynamic_cast<PuzzleItem *>(graphicsItem);
                    if(item->movable()) {
                        emptyPlace_ = item->currentPlace();
                        pieces_.at(hiddenIndex_)->setCurrentPlace(item->currentPlace());
                        pieces_.at(hiddenIndex_)->setPos(item->currentPlace());
                        item->setCurrentPlace(tmp);
                        item->setPos(tmp);
                        setMovingPieces();
                        movesMade++;
                    }
                    else {
                        qDebug() << "Item up of hidden piece not movable";
                    }
                }
            }
            else {
                --i;
            }
            break;
        default:
            qDebug() << "WTF?";
            break;
        }
    }

    qDebug() << QString("Shuffle moves: %1/%2").arg(movesMade).arg(moveCount);

    QParallelAnimationGroup *animationGroup = new QParallelAnimationGroup(this);
    connect(animationGroup, SIGNAL(finished()), this, SLOT(shuffleAnimationFinished()));
    for(int i = 0; i < pieces_.count(); ++i) {
        QPropertyAnimation *animation = new QPropertyAnimation(pieces_.at(i), "pos");
        animation->setStartValue(pieces_.at(i)->correctPlace());
        animation->setEndValue(pieces_.at(i)->currentPlace());
        animation->setDuration(750);
        animation->setEasingCurve(QEasingCurve::InOutCirc);
        animationGroup->addAnimation(animation);
    }
    animationGroup->start();
    pieces_.at(hiddenIndex_)->hide();
}

void GameView::shuffleAnimationFinished()
{
    setMovingPieces();
    PuzzleItem::setManuallyMovable(true);
}

QPointF GameView::emptyPlace()
{
    return emptyPlace_;
}

void GameView::setEmptyPlace(const QPointF &place)
{
    emptyPlace_ = place;
}

bool GameView::areAllPiecesOk()
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
    emit gameWon();

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
        bool ok = false;
        int pieces = list.at(0).toInt(&ok);
        if(!ok) {
            return false;
        }

        QString im = list.at(1);
        if(!QFile::exists(im) && im != "default") {
            return false;
        }

        int moveCount = list.at(2).toInt(&ok);
        if(!ok) {
            return false;
        }

        Settings::instance()->setPieceCount(pieces);
        PuzzleItem::setMoveCount(moveCount);

        if(im == "default" || im.isEmpty()) {
            Settings::instance()->setImage(QPixmap());
            Settings::instance()->setImagePath("default");
        }
        else {
            Settings::instance()->setImagePath(im);
            Settings::instance()->setImage(QPixmap(im));
        }

        setPieces(ImageImporter::instance()->newPieces(Settings::instance()->image(), Settings::instance()->pieceCount()), false);

        qDebug() << "pieces_ count after restoring image: " << pieces_.count();

        if(list.count() >= pieces_.count() + 3) {
            for(int j = 0; j < pieces_.count(); ++j) {
                if(!list.at(j + 3).isNull()) {
                    QStringList points = list.at(j + 3).split("#");

                    int x = points.at(0).toInt(&ok);
                    if(!ok) {
                        return false;
                    }

                    int y = points.at(1).toInt(&ok);
                    if(!ok) {
                        return false;
                    }

                    QPointF point(x, y);

                    //qDebug() << "Setting piece " << pieces_.at(j)->pieceNumber();
                    //qDebug() << "x: " << point.x() << " y: " << point.y();

                    pieces_.at(j)->setCurrentPlace(point);
                }
                else {
                    return false;
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
                    //qDebug() << "Hiding piece number " << hidden;
                    hiddenIndex_ = m;
                }
            }

            setEmptyPlace(QPointF(hidden.at(0).toInt(), hidden.at(1).toInt()));

            pieces_.at(hiddenIndex_)->setVisible(false);

            setMovingPieces();
        }
        else {
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

    QFileInfo fileInfo(file);

    QDateTime created = fileInfo.created();
    QString infoTxt = QString("Restored game state from %1")
                      .arg(created.toString(Qt::TextDate));

#ifdef Q_WS_MAEMO_5
    QMaemo5InformationBox::information(this, infoTxt);
#endif

    file.close();
    file.remove();

    return true;
}

void GameView::saveGame()
{
    if(pieces_.isEmpty() || pieces_.count() < EASY_PIECE_COUNT) {
        return;
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
        return;
    }

    QTextStream out(&file);

    out << Settings::instance()->pieceCount();
    out << QString(";;");
    if(Settings::instance()->imagePath().isEmpty()) {
        out << QString("default");
    }
    else {
        out << Settings::instance()->imagePath();
    }
    out << QString(";;");
    out << PuzzleItem::moveCount();
    out << QString(";;");

    // piece positions
    int number = 0;
    int hiddenNo = 0;

    while(number != pieces_.count()) {
        for(int i = 0; i < pieces_.count(); ++i) {
            if(pieces_.at(i)->pieceNumber() == number + 1) {
                out << pieces_.at(i)->currentPlace().x();
                out << QString("#");
                out << pieces_.at(i)->currentPlace().y();
                out << QString(";;");
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

    qApp->quit();
}

int GameView::correctPlaces() const
{
    int c = 0;

    for(int i = 0; i < pieces_.count(); ++i) {
        if(pieces_.at(i)->currentPlace() == pieces_.at(i)->correctPlace()) {
            c++;
        }
    }

    return c;
}

QList<int> GameView::movingPlaces() const
{
    QList<int> m;

    for(int i = 0; i < pieces_.count(); ++i) {
        if(pieces_.at(i)->movable()) {
            m.append(i);
        }
    }

    return m;
}
