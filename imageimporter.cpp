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
    QPixmap tmp;

    if(pixmap.isNull()) {
        qDebug() << "using default image";
        tmp = QPixmap(":/images/default.jpg");
    }
    else {
        tmp = pixmap;
    }

    QList<QPixmap> list;

    int horizontalCount = 0;

    if(count == EASY_PIECE_COUNT) {
        horizontalCount = EASY_HORIZONTAL_COUNT;
    }
    else if(count == HARD_PIECE_COUNT) {
        horizontalCount = HARD_HORIZONTAL_COUNT;
    }
    else {
        qDebug() << QString("Bad piece count ( %1 ) @ ImageImporter::newPieces").arg(count);
        return list;
    }

    int verticalCount = count / horizontalCount;
    int verticalStep = tmp.height() / verticalCount;
    int horizontalStep = tmp.width() / horizontalCount;

    for(int i = 0; i < verticalCount; ++i) {
        for(int j = 0; j < horizontalCount; ++j) {
            QPixmap item;
            item = tmp.copy(QRect(QPoint(j * horizontalStep, i * verticalStep),
                                           QPoint(horizontalStep + j * horizontalStep, verticalStep + i * verticalStep)));

            list.append(item);
        }
    }

    return list;
}
