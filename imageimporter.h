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

#ifndef IMAGEIMPORTER_H
#define IMAGEIMPORTER_H

#include <QObject>

#include "defines.h"

class QPixmap;
class PuzzleItem;

class ImageImporter : public QObject
{
    Q_OBJECT

public:
    static ImageImporter *instance();
    QList<QPixmap> newPieces(const QPixmap &pixmap, const int count = EASY_PIECE_COUNT);

private:
    ImageImporter(QObject *parent = 0);

    static ImageImporter *p_instance;
};
#endif
