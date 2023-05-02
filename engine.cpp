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

    foreach (QString directory, directories) {
        QDirIterator di(directory, nameFilters, filters, flags);
        QString tmpLocation;
        QImage tmpImage;
        while(di.hasNext()) {
            tmpLocation = di.next();
            tmpImage.load(tmpLocation);
            if(tmpImage.width() > 300 && tmpImage.height() > 200 && tmpLocation != "") {
                QString fileString = "file:" + tmpLocation;
                if(!m_imageList.contains(fileString)) {
                    m_imageList << fileString;
                }
            }
            if(m_imageList.count() > 30) break;
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

    if(m_itemList.count() == 9) {
        switch (index) {
        case 0: if(PuzzleItem::hiddenIndex() == 1  || PuzzleItem::hiddenIndex() == 3) return true;
            else return false;
            break;
        case 1: if(PuzzleItem::hiddenIndex() == 0  || PuzzleItem::hiddenIndex() == 2 || PuzzleItem::hiddenIndex() == 4) return true;
            else return false;
            break;
        case 2: if(PuzzleItem::hiddenIndex() == 1  || PuzzleItem::hiddenIndex() == 5) return true;
            else return false;
            break;
        case 3: if(PuzzleItem::hiddenIndex() == 0  || PuzzleItem::hiddenIndex() == 4 || PuzzleItem::hiddenIndex() == 6) return true;
            else return false;
            break;
        case 4: if(PuzzleItem::hiddenIndex() == 1  || PuzzleItem::hiddenIndex() == 3 || PuzzleItem::hiddenIndex() == 5 || PuzzleItem::hiddenIndex() == 7) return true;
            else return false;
            break;
        case 5: if(PuzzleItem::hiddenIndex() == 2  || PuzzleItem::hiddenIndex() == 4 || PuzzleItem::hiddenIndex() == 8) return true;
            else return false;
            break;
        case 6: if(PuzzleItem::hiddenIndex() == 3  || PuzzleItem::hiddenIndex() == 7) return true;
            else return false;
            break;
        case 7: if(PuzzleItem::hiddenIndex() == 6  || PuzzleItem::hiddenIndex() == 4 || PuzzleItem::hiddenIndex() == 8) return true;
            else return false;
            break;
        case 8: if(PuzzleItem::hiddenIndex() == 5  || PuzzleItem::hiddenIndex() == 7) return true;
            else return false;
            break;
        default:
            return false;
        }
    }

    /*
      0  1  2  3
      4  5  6  7
      8  9 10 11
     12 13 14 15
     */

    else if(m_itemList.count() == 16) {
        switch (index) {
        case 0: if(PuzzleItem::hiddenIndex() == 1 || PuzzleItem::hiddenIndex() == 4) return true;
            else return false;
            break;
        case 1: if(PuzzleItem::hiddenIndex() == 0 || PuzzleItem::hiddenIndex() == 2 || PuzzleItem::hiddenIndex() == 5) return true;
            else return false;
            break;
        case 2: if(PuzzleItem::hiddenIndex() == 1 || PuzzleItem::hiddenIndex() == 3 || PuzzleItem::hiddenIndex() == 6) return true;
            else return false;
            break;
        case 3: if(PuzzleItem::hiddenIndex() == 2 || PuzzleItem::hiddenIndex() == 7) return true;
            else return false;
            break;
        case 4: if(PuzzleItem::hiddenIndex() == 0 || PuzzleItem::hiddenIndex() == 5 || PuzzleItem::hiddenIndex() == 8) return true;
            else return false;
            break;
        case 5: if(PuzzleItem::hiddenIndex() == 1 || PuzzleItem::hiddenIndex() == 4 || PuzzleItem::hiddenIndex() == 6 || PuzzleItem::hiddenIndex() == 9) return true;
            else return false;
            break;
        case 6: if(PuzzleItem::hiddenIndex() == 2 || PuzzleItem::hiddenIndex() == 5 || PuzzleItem::hiddenIndex() == 7 || PuzzleItem::hiddenIndex() == 10) return true;
            else return false;
            break;
        case 7: if(PuzzleItem::hiddenIndex() == 3 || PuzzleItem::hiddenIndex() == 6 || PuzzleItem::hiddenIndex() == 11) return true;
            else return false;
            break;
        case 8: if(PuzzleItem::hiddenIndex() == 4 || PuzzleItem::hiddenIndex() == 9 || PuzzleItem::hiddenIndex() == 12) return true;
            else return false;
            break;
        case 9: if(PuzzleItem::hiddenIndex() == 5 || PuzzleItem::hiddenIndex() == 8 || PuzzleItem::hiddenIndex() == 10 || PuzzleItem::hiddenIndex() == 13) return true;
            else return false;
            break;
        case 10: if(PuzzleItem::hiddenIndex() == 6 || PuzzleItem::hiddenIndex() == 9 || PuzzleItem::hiddenIndex() == 11 || PuzzleItem::hiddenIndex() == 14) return true;
            else return false;
            break;
        case 11: if(PuzzleItem::hiddenIndex() == 7 || PuzzleItem::hiddenIndex() == 10 || PuzzleItem::hiddenIndex() == 15) return true;
            else return false;
            break;
        case 12: if(PuzzleItem::hiddenIndex() == 8 || PuzzleItem::hiddenIndex() == 13) return true;
            else return false;
            break;
        case 13: if(PuzzleItem::hiddenIndex() == 9 || PuzzleItem::hiddenIndex() == 12 || PuzzleItem::hiddenIndex() == 14) return true;
            else return false;
            break;
        case 14: if(PuzzleItem::hiddenIndex() == 13 || PuzzleItem::hiddenIndex() == 10 || PuzzleItem::hiddenIndex() == 15) return true;
            else return false;
            break;
        case 15: if(PuzzleItem::hiddenIndex() == 11 || PuzzleItem::hiddenIndex() == 14) return true;
            else return false;
            break;
        default:
            return false;
        }
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
