#ifndef WALL_H
#define WALL_H

#include <QGraphicsItem>

class Wall : public QGraphicsItem
{
public:
    Wall();
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;
};

#endif // WALL_H
