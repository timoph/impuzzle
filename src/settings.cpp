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

#include "settings.h"
#include "defines.h"

Settings *Settings::instance_ = 0;

Settings::Settings(QObject *parent) :
        QObject(parent)
{
    pieceCount_ = EASY_PIECE_COUNT;
    image_ = 0;
    imagePath_ = "";
}

Settings *Settings::instance()
{
    if(!instance_) {
        instance_ = new Settings;
    }

    return instance_;
}

int Settings::pieceCount() const
{
    return pieceCount_;
}

void Settings::setPieceCount(const int pieces)
{
    pieceCount_ = pieces;
}

QPixmap Settings::image() const
{
    return image_;
}

void Settings::setImage(const QPixmap &image)
{
    image_ = image;
}

QString Settings::imagePath() const
{
    return imagePath_;
}

void Settings::setImagePath(const QString &path)
{
    imagePath_ = path;
}

QStringList Settings::localImages() const
{
    return imageList_;
}

void Settings::setLocalImages(const QStringList &files)
{
    imageList_ = files;
}
