#include "introitem.h"
#include "defines.h"

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

    painter->setBrush(Qt::black);
    painter->setPen(Qt::NoPen);
    painter->drawRect(boundingRect());

    painter->setBrush(Qt::NoBrush);
    painter->setPen(Qt::white);

    // Get font metrics
    QFontMetricsF fontMetricsF(painter->font());
    QRectF textRectF = fontMetricsF.boundingRect(text_);
    int horizontalIntend = (IMAGE_WIDTH - textRectF.width()) / 2;
    int verticalIntend = (IMAGE_HEIGHT - textRectF.height()) / 2;

    // Draw text aligned to the center of boundingRect
    painter->drawText(boundingRect()
                      .adjusted(horizontalIntend, verticalIntend,-horizontalIntend, -verticalIntend),
                      text_);

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
