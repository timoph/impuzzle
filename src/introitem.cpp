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

#include "introitem.h"
#include "defines.h"
#include "mainwindow.h"

#include <QPainter>
#include <QFontMetricsF>

IntroItem::IntroItem(QGraphicsItem *parent) :
        QGraphicsItem(parent)
{
    text_ = "";
}

QRectF IntroItem::boundingRect() const
{
    return QRectF(0, 0, IMAGE_WIDTH, IMAGE_HEIGHT);
}

void IntroItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option)
    Q_UNUSED(widget)

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);
    painter->setPen(Qt::NoPen);

    const int dots = 96;

    int hstep = boundingRect().width() / dots;
    int vstep = boundingRect().height() / dots;

    QColor colors[3] = {QColor(255, 255, 255), QColor(0, 0, 0), QColor(127, 127, 127)};

    for(int i = 0; i < dots; ++i) {
        for(int j = 0; j < dots; ++j) {
            painter->setBrush(QBrush(colors[qrand() % 2]));
            painter->drawRect(QRect(QPoint(i * hstep, j * vstep),
                                    QPoint((i+1) * hstep, (j+1) * vstep)));
        }
    }

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::black);

    QFont font = painter->font();
    font.setPointSize(72);
    font.setBold(true);
    painter->setFont(font);

    // Get font metrics
    QFontMetricsF fontMetricsF(painter->font());
    QRectF textRectF = fontMetricsF.boundingRect(text_);
    int horizontalIntend = (boundingRect().width() - textRectF.width()) / 2 - 10;
    int verticalIntend = (boundingRect().height() - textRectF.height()) / 2 - 10;

    // Draw rect behind the text
    painter->setBrush(QBrush(QColor(0,0,0,192)));
    painter->setPen(Qt::NoPen);
    painter->drawRect(boundingRect()
                      .adjusted(0, verticalIntend, 5, -verticalIntend));

    painter->setPen(Qt::black);
    painter->setBrush(Qt::NoBrush);
    // Draw text aligned to the center of boundingRect
    QPen pen = painter->pen();
    pen.setCosmetic(true);
    pen.setColor(QColor(255,20,0,192));

    painter->setPen(pen);
    painter->drawText(boundingRect()
                      .adjusted(horizontalIntend, verticalIntend,-horizontalIntend, -verticalIntend),
                      text_);

    pen.setColor(QColor(Qt::white));
    painter->setPen(pen);
    font.setPointSize(12);
    painter->setFont(font);

    QString txt = QString("Version %1").arg(IMPUZZLE_VERSION);

    QFontMetricsF fm(painter->font());
    QRectF trect = fm.boundingRect(txt);

    QTextOption textOption;
    textOption.setAlignment(Qt::AlignRight);

    painter->drawText(boundingRect().adjusted(0, verticalIntend * 2 - trect.height(), -trect.width(), 0),
                      txt, textOption);

    painter->restore();
}

QString IntroItem::text() const
{
    return text_;
}

void IntroItem::setText(const QString &txt)
{
    text_ = txt;

    if(isVisible()) {
        update();
    }
}

void IntroItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);

    MainWindow::instance()->newGameClicked();
}
