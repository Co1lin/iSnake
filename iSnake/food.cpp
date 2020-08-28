#include <QPainter>
#include "food.h"
#include "parameters.h"

static const qreal FOOD_RADIUS = 5;

Food::Food(qreal x, qreal y)
{
    setPos(x, y);
    qDebug() << "add a Food: " << this << ": " << x << ", " << y;
}

QRectF Food::boundingRect() const
{
    return QRectF(mapFromScene(QPointF(0, 0)), QSizeF(BLOCK_SIZE, BLOCK_SIZE));
}

void Food::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->save();

    painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::red);

    painter->restore();
}

QPainterPath Food::shape() const
{
    QPainterPath p;
    p.addEllipse(QPointF(BLOCK_SIZE / 2.0 + fixCenter, BLOCK_SIZE / 2.0 + fixCenter), FOOD_RADIUS, FOOD_RADIUS);
    // p.addRect(QRectF(0, 0, BLOCK_SIZE, BLOCK_SIZE));
    return p;
}



QDataStream& operator>>(QDataStream& in, Food& _food)
{
    qreal x, y;
    in >> x >> y;
    _food.setX(x);
    _food.setY(y);
    return in;
}

QDataStream& operator<<(QDataStream& out, const Food& _food)
{
    out << _food.x() << _food.y();
    return out;
}
