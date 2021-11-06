/*
 ImPuzzle 2 - a 15-puzzle inspired game
 Copyright (C) 2021 Timo Härkönen

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

#include "puzzleitem.h"

#include <QPainter>
#include <QFont>
#include <QFontMetrics>

#include "engine.h"

#include <QDebug>

int PuzzleItem::m_hiddenIndex = 0;

PuzzleItem::PuzzleItem(QQuickPaintedItem *parent) :
        QQuickPaintedItem(parent)
{
    m_isMovable = true;
    m_currentIndex= 0;
    m_correctIndex = 0;
    m_drawNumber = true;

    connect(Engine::instance(), SIGNAL(update()),
            this, SLOT(update()));
}

bool PuzzleItem::movable() const
{
    return m_isMovable;
}

void PuzzleItem::setMovable(bool canMove)
{
    if(canMove != m_isMovable) {
        m_isMovable = canMove;
        emit movableChanged();
    }
}

void PuzzleItem::paint(QPainter *painter)
{
    if(!Engine::instance()->gameRunning()) return;

    painter->setOpacity(opacity());

    painter->setPen(Qt::NoPen);
    if(m_currentIndex == m_hiddenIndex) {
        painter->save();
        painter->setBrush(Qt::black);
        painter->drawRect(0, 0, width(), height());
        painter->restore();
    }
    else {
        painter->drawPixmap(0, 0, width(), height(), m_pixmap);
    }

    if(m_drawNumber && m_correctIndex >= 0) {
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

        painter->drawText(numberRect, QString::number(m_correctIndex + 1), textOption);

        painter->restore();
    }
}

int PuzzleItem::currentIndex() const
{
    return m_currentIndex;
}

int PuzzleItem::correctIndex() const
{
    return m_correctIndex;
}

void PuzzleItem::setCurrentIndex(const int newIndex)
{
    if(newIndex != m_currentIndex) {
        m_currentIndex = newIndex;
        emit currentIndexChanged();
        update();
    }
}

void PuzzleItem::setCorrectIndex(const int pieceIndex)
{
    if(pieceIndex != m_correctIndex) {
        m_correctIndex = pieceIndex;
        emit correctIndexChanged();
    }
}

void PuzzleItem::setDrawNumber(bool value)
{
    if(value != m_drawNumber) {
        m_drawNumber = value;
        emit drawNumberChanged();
        update();
    }
}

void PuzzleItem::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
    emit pixmapChanged();
    update();
}

void PuzzleItem::setHiddenIndex(const int index)
{
    if(index != m_hiddenIndex) {
        m_hiddenIndex = index;
    }
}


bool PuzzleItem::drawNumber() const
{
    return m_drawNumber;
}

QPixmap PuzzleItem::pixmap() const
{
    return m_pixmap;
}

int PuzzleItem::hiddenIndex()
{
    return m_hiddenIndex;
}

