#ifndef INTROITEM_H
#define INTROITEM_H

#include <QGraphicsItem>

class IntroItem : public QGraphicsItem
{
public:
    IntroItem(QGraphicsItem *parent = 0);
    QRectF boundingRect() const;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
    QString text() const;
    void setText(const QString &txt);

private:
    QString text_;
};
#endif
