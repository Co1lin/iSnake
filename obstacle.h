#ifndef OBSTACLE_H
#define OBSTACLE_H

#include <QGraphicsItem>

class Obstacle : public QGraphicsItem
{
public:
    Obstacle();

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;

    const int REAL_OBSTACLE_SIZE = 10;
    const qreal fixCenter = 1;

    void change(const QPointF& pos);

    friend QDataStream& operator>>(QDataStream& in, Obstacle& _obstacle);

    friend QDataStream& operator<<(QDataStream& out, const Obstacle& _obstacle);

private:
    QList<QPointF> obstacles;
};

#endif // OBSTACLE_H
