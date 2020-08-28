#include <QPainter>
#include "wall.h"
#include "parameters.h"

Wall::Wall()
{
    setPos(0, 0);
}

QRectF Wall::boundingRect() const
{
    return QRectF(0, 0, MAP_SIZE, MAP_SIZE);
}

void Wall::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->save();

    // painter->setRenderHint(QPainter::Antialiasing);
    painter->fillPath(shape(), Qt::blue);

    painter->restore();
}

QPainterPath Wall::shape() const
{
    QPainterPath p;

    const int REAL_WALL_SIZE = 8;
    p.addRect(0, 0, MAP_SIZE, REAL_WALL_SIZE);
    p.addRect(MAP_SIZE - REAL_WALL_SIZE, REAL_WALL_SIZE, REAL_WALL_SIZE, MAP_SIZE - REAL_WALL_SIZE);
    p.addRect(0, MAP_SIZE - REAL_WALL_SIZE, MAP_SIZE - REAL_WALL_SIZE, REAL_WALL_SIZE);
    p.addRect(0, REAL_WALL_SIZE, REAL_WALL_SIZE, MAP_SIZE - 2 * REAL_WALL_SIZE);

    return p;
}
