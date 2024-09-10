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

#include "engine.h"
#include "defines.h"

#include <QRandomGenerator>
#include <QtGlobal>
#include <QDirIterator>
#include <QTimer>
#include <QDebug>

Engine *Engine::p_instance = 0;

Engine *Engine::instance()
{
    if(!p_instance) {
        p_instance = new Engine;
    }
    return p_instance;
}

void Engine::start(bool morePieces)
{
    m_gameStarted = false;
    m_itemList.clear();
    m_moveCount = 0;
    int count = EASY_PIECE_COUNT;
    if(morePieces) count = HARD_PIECE_COUNT;
    QList<QPixmap> tmpList = ImageImporter::instance()->newPieces(m_image, count);
    for(int i = 0; i < tmpList.count(); i++) {
        m_itemList.append(QPair<int, QPixmap>(i, tmpList.at(i)));
    }
    PuzzleItem::setHiddenIndex(0);
    sufflePieces();
    m_gameStarted = true;
    emit gameStarted();
    emit imagesImported();
}

bool Engine::gameRunning() const
{
    return m_gameStarted;
}

QStringList Engine::imageList() const
{
    return m_imageList;
}

void Engine::setShuffleCount(int shuffleCount)
{
    if(shuffleCount > 0) {
        m_shuffleCount = shuffleCount;
    }
}

void Engine::setSearchPath(QString path)
{
    m_searchPath = path;
}

void Engine::setUserImageFile(QString filePath)
{
    m_userImageFile = filePath;
    setImage(QUrl::fromLocalFile(m_userImageFile));
}

QPixmap Engine::imageForIndex(int index) const
{
    if(index >= 0 && index < m_itemList.count()) {
        return m_itemList.at(index).second;
    }
    return QPixmap();
}

int Engine::correctPositionForIndex(int index) const
{
    if(index >= 0 && index < m_itemList.count()) {
        return m_itemList.at(index).first;
    }

    return -1;
}

bool Engine::pieceClicked(int index)
{
    if(index != PuzzleItem::hiddenIndex() && pieceCanMove(index)) {
        m_moveCount++;
#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
        m_itemList.swapItemsAt(PuzzleItem::hiddenIndex(), index);
#else
        m_itemList.swap(PuzzleItem::hiddenIndex(), index);
#endif

        if(isPuzzleSolved()) {
            qDebug() << "puzzle pieces in correct positions";
            PuzzleItem::setHiddenIndex(-1);
            emit puzzleSolved(m_moveCount);
        }
        else {
            PuzzleItem::setHiddenIndex(index);
        }
        emit update();
        return true;
    }
    else {
        return false;
    }
}

void Engine::setImage(QUrl imageUrl)
{
    m_imageUrl = imageUrl;

    emit imageUrlChanged();

    QTimer::singleShot(500, this, SLOT(handleImageChange()));
}

QUrl Engine::currentImage() const
{
    return m_imageUrl;
}

void Engine::findImages()
{
    m_imageList.clear();
    QStringList nameFilters;
    nameFilters << "*.jpg" << "*.png";
    QDir::Filters filters = QDir::NoFilter;
    QDirIterator::IteratorFlags flags = QDirIterator::Subdirectories;

    QStringList directories;
    if(m_searchPath == "") {
        directories << QDir::currentPath() << QDir::homePath();
    }
    else {
        directories << m_searchPath;
    }

    m_imageList << "qrc:/images/default.jpg";

    for (const QString& directory : directories) {
        QDirIterator di(directory, nameFilters, filters, flags);
        QString tmpLocation;
        QImage tmpImage;
        while (di.hasNext() && m_imageList.count() <= 30) {
            tmpLocation = di.next();
            tmpImage.load(tmpLocation);
            if (tmpImage.width() > 300 && tmpImage.height() > 200 && !tmpLocation.isEmpty()) {
                QString fileString = "file:" + tmpLocation;
                if (!m_imageList.contains(fileString)) {
                    m_imageList.append(fileString);
                }
            }
        }
    }

    emit imageListChanged();
}

bool Engine::pieceCanMove(int index) const
{
    /*
     0 1 2
     3 4 5
     6 7 8
     */    
    
    if (m_itemList.count() == EASY_PIECE_COUNT) {
        static const std::unordered_map<int, std::vector<int>> validMoves = {
            {0, {1, 3}},
            {1, {0, 2, 4}},
            {2, {1, 5}},
            {3, {0, 4, 6}},
            {4, {1, 3, 5, 7}},
            {5, {2, 4, 8}},
            {6, {3, 7}},
            {7, {4, 6, 8}},
            {8, {5, 7}}
        };

        auto it = validMoves.find(index);
        if (it != validMoves.end()) {
            for (int move : it->second) {
                if (PuzzleItem::hiddenIndex() == move) {
                    return true;
                }
            }
        }
        return false;
    }

    /*
      0  1  2  3
      4  5  6  7
      8  9 10 11
     12 13 14 15
     */

    else if (m_itemList.count() == HARD_PIECE_COUNT) {
        static const std::unordered_map<int, std::vector<int>> validBiggerGameMoves = {
            {0, {1, 4}},
            {1, {0, 2, 5}},
            {2, {1, 3, 6}},
            {3, {2, 7}},
            {4, {0, 5, 8}},
            {5, {1, 4, 6, 9}},
            {6, {2, 5, 7, 10}},
            {7, {3, 6, 11}},
            {8, {4, 9, 12}},
            {9, {5, 8, 10, 13}},
            {10, {6, 9, 11, 14}},
            {11, {7, 10, 15}},
            {12, {8, 13}},
            {13, {9, 12, 14}},
            {14, {10, 13, 15}},
            {15, {11, 14}}
        };

        auto it = validBiggerGameMoves.find(index);
        if (it != validBiggerGameMoves.end()) {
            for (int move : it->second) {
                if (PuzzleItem::hiddenIndex() == move) {
                    return true;
                }
            }
        }
        return false;
    }

    return false;
}

bool Engine::isPuzzleSolved() const
{
    if(!m_gameStarted) return false;

    bool solved = true;

    for(int i = 0; i < m_itemList.count(); i++) {
        if(m_itemList.at(i).first != i) {
            solved = false;
        }
    }

    return solved;
}

void Engine::sufflePieces()
{
    int succeededShuffles = 0;

    while(succeededShuffles < m_shuffleCount) {
         if(pieceClicked(QRandomGenerator::global()->bounded(0, m_itemList.count()))) succeededShuffles++;
    }

    m_moveCount = 0;
}

void Engine::handleImageChange()
{
    if(m_imageUrl.toString().startsWith("qrc")) {
        // for qrc url removing ":" from scheme is not wanted so this hack exists
        m_image.load(m_imageUrl.toString().remove(0, 3));
    }
    else {
        m_image.load(m_imageUrl.toString(QUrl::RemoveScheme | QUrl::NormalizePathSegments));
    }

    emit imageLoaded();
}

Engine::Engine(QObject *parent) : QObject(parent)
{
    m_moveCount = 0;
    m_gameStarted = false;
    m_imageUrl = "qrc:/images/default.jpg";
    m_shuffleCount = 40;
    m_userImageFile = "";
    m_searchPath = "";
}
