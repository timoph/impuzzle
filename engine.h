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

#ifndef ENGINE_H
#define ENGINE_H

#include <QObject>
#include <QPixmap>

#include "puzzleitem.h"
#include "imageimporter.h"

class Engine : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList imageList READ imageList NOTIFY imageListChanged)

public:
    static Engine *instance();
    Q_INVOKABLE void start(bool morePieces);
    bool gameRunning() const;
    QStringList imageList() const;
    void setShuffleCount(int shuffleCount);
    void setSearchPath(QString path);
    void setUserImageFile(QString filePath);

public slots:
    Q_INVOKABLE QPixmap imageForIndex(int index) const;
    Q_INVOKABLE int correctPositionForIndex(int index) const;
    Q_INVOKABLE bool pieceClicked(int index);
    Q_INVOKABLE void setImage(QUrl imageUrl);
    Q_INVOKABLE QUrl currentImage() const;
    Q_INVOKABLE void findImages();

signals:
    void imagesImported();
    void update();
    void hiddenIndexChanged(int index);
    void puzzleSolved(int moveCount);
    void gameStarted();
    void imageUrlChanged();
    void imageListChanged();
    void imageLoaded();

private slots:
    void handleImageChange();

private:
    bool pieceCanMove(int index) const;
    bool isPuzzleSolved() const;
    void sufflePieces();

private:
    explicit Engine(QObject *parent = nullptr);
    static Engine *p_instance;
    QList<QPair<int, QPixmap> > m_itemList;
    int m_moveCount;
    bool m_gameStarted;
    QPixmap m_image;
    QUrl m_imageUrl;
    QStringList m_imageList;
    int m_shuffleCount;
    QString m_userImageFile;
    QString m_searchPath;
};

#endif // ENGINE_H
