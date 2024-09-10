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

#include "imageimporter.h"
#include "puzzleitem.h"
#include "defines.h"

#include <QPixmap>

#include <QDebug>

ImageImporter *ImageImporter::p_instance = 0;

ImageImporter::ImageImporter(QObject *parent) :
        QObject(parent)
{

}

ImageImporter *ImageImporter::instance()
{
    if(!p_instance) {
        p_instance = new ImageImporter;
    }

    return p_instance;
}

QList<QPixmap> ImageImporter::newPieces(const QPixmap &pixmap, const int count)
{
    QPixmap tmp = pixmap.isNull() ? QPixmap(":/images/default.jpg") : pixmap;

    QList<QPixmap> list;

    int horizontalCount = 0;
    horizontalCount = count == EASY_PIECE_COUNT ? EASY_HORIZONTAL_COUNT : HARD_HORIZONTAL_COUNT;

    const int verticalCount = count / horizontalCount;
    const int verticalStep = tmp.height() / verticalCount;
    const int horizontalStep = tmp.width() / horizontalCount;

    list.reserve(verticalCount * horizontalCount);

    for(int i = 0; i < verticalCount; ++i) {
        for(int j = 0; j < horizontalCount; ++j) {
            QPixmap item = tmp.copy(QRect(QPoint(j * horizontalStep, i * verticalStep),
                                          QPoint(horizontalStep + j * horizontalStep, verticalStep + i * verticalStep)));

            list.append(std::move(item));
        }
    }

    return list;
}
