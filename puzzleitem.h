/*
 ImPuzzle 2 - a 15-puzzle inspired game
 Copyright (C) 2023 Timo Rautiainen

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef PUZZLEITEM_H
#define PUZZLEITEM_H

#include <QQuickPaintedItem>
#include <QPixmap>

class QPropertyAnimation;

class PuzzleItem : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(bool movable READ movable WRITE setMovable NOTIFY movableChanged)
    Q_PROPERTY(int currentIndex READ currentIndex WRITE setCurrentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(bool drawNumber READ drawNumber WRITE setDrawNumber NOTIFY drawNumberChanged)
    Q_PROPERTY(QPixmap pixmap READ pixmap WRITE setPixmap NOTIFY pixmapChanged)
    Q_PROPERTY(int correctIndex READ correctIndex WRITE setCorrectIndex NOTIFY correctIndexChanged)

public:
    PuzzleItem(QQuickPaintedItem *parent = 0);
    bool movable() const;
    void paint(QPainter *painter);
    int currentIndex() const;
    int correctIndex() const;
    bool drawNumber() const;
    QPixmap pixmap() const;
    static int hiddenIndex();

public slots:
    void setCurrentIndex(const int newIndex);
    void setCorrectIndex(const int pieceIndex);
    void setMovable(bool canMove);
    void setDrawNumber(bool value);
    void setPixmap(QPixmap pixmap);
    static void setHiddenIndex(const int index);

signals:
    void movableChanged();
    void currentIndexChanged();
    void drawNumberChanged();
    void pixmapChanged();
    void correctIndexChanged();
    void opacityChanged();

private:
    bool m_isMovable;
    int m_currentIndex;
    int m_correctIndex;
    bool m_drawNumber;
    static int m_hiddenIndex;
    QPixmap m_pixmap;
};
#endif
