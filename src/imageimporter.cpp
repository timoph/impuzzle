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

#include "imageimporter.h"
#include "puzzleitem.h"
#include "defines.h"

#include <QPixmap>

#include <QDebug>

ImageImporter *ImageImporter::instance_ = 0;

ImageImporter::ImageImporter(QObject *parent) :
        QObject(parent)
{

}

ImageImporter *ImageImporter::instance()
{
    if(!instance_) {
        instance_ = new ImageImporter;
    }

    return instance_;
}

QList<PuzzleItem *> ImageImporter::newPieces(const QPixmap &pixmap, const int count)
{
    QPixmap tmp;

    if(pixmap.isNull()) {
        qDebug() << "Got NULL image - using default.jpg";
        tmp = QPixmap(":/images/default.jpg");
    }
    else {
        tmp = pixmap;
    }

    if(tmp.size().height() != IMAGE_HEIGHT || tmp.size().width() != IMAGE_WIDTH) {
        tmp = pixmap.scaled(QSize(IMAGE_WIDTH, IMAGE_HEIGHT), Qt::KeepAspectRatioByExpanding);
    }

    QList<PuzzleItem *> list;

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
    int verticalStep = IMAGE_HEIGHT / verticalCount;
    int horizontalStep = IMAGE_WIDTH / horizontalCount;

    int pieceNo = 1;

    for(int i = 0; i < verticalCount; ++i) {
        for(int j = 0; j < horizontalCount; ++j) {
            PuzzleItem *item = new PuzzleItem;
            item->setPixmap(tmp.copy(QRect(QPoint(j * horizontalStep, i * verticalStep),
                                           QPoint(horizontalStep + j * horizontalStep, verticalStep + i * verticalStep))));
            item->setPieceNumber(pieceNo);
            pieceNo++;
            list.append(item);
        }
    }

    return list;
}
