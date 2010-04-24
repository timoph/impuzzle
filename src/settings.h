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

#ifndef SETTINGS_H
#define SETTINGS_H

#include <QObject>
#include <QPixmap>

class Settings : public QObject
{
    Q_OBJECT

public:
    static Settings *instance();

    int pieceCount() const;
    void setPieceCount(const int pieces);

    QPixmap image() const;
    void setImage(const QPixmap &image);

    QString imagePath() const;
    void setImagePath(const QString &path);

private:
    Settings(QObject *parent = 0);

    static Settings *instance_;

    int pieceCount_;
    QPixmap image_;
    QString imagePath_;
};
#endif
