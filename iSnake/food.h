#ifndef FOOD_H
#define FOOD_H

#include <QGraphicsItem>

class Food : public QGraphicsItem
{
public:
    Food() {}
    Food(qreal x, qreal y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;

    friend QDataStream& operator>>(QDataStream& in, Food& _food);
    friend QDataStream& operator<<(QDataStream& out, const Food& _food);
};

#endif // FOOD_H
