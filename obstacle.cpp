#include <QPainter>
#include <QGraphicsScene>
#include "obstacle.h"
#include "parameters.h"

Obstacle::Obstacle()
{

}

QRectF Obstacle::boundingRect() const
{
//    qreal minX = obstacles.first().x();
//    qreal minY = obstacles.first().y();
//    qreal maxX = obstacles.first().x();
//    qreal maxY = obstacles.first().y();

//    foreach (auto& p, obstacles)
//    {
//        maxX = p.x() > maxX ? p.x() : maxX;
//        maxY = p.y() > maxY ? p.y() : maxY;
//        minX = p.x() < minX ? p.x() : minX;
//        minY = p.y() < minY ? p.y() : minY;
//    }

//    QPointF tl = mapFromScene(QPointF(minX, minY));
//    QPointF br = mapFromScene(QPointF(maxX, maxY));

//    QRectF bound = QRectF(tl.x() + fixCenter,  // x
//                          tl.y() + fixCenter,  // y
//                          br.x() - tl.x() - 2 * fixCenter, // width
//                          br.y() - tl.y() - 2 * fixCenter  // height
//                          );
//    return bound;
    return QRectF(mapFromScene(QPointF(0, 0)), QSizeF(MAP_MOVE_SIZE, MAP_MOVE_SIZE));
}

QPainterPath Obstacle::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    foreach (auto& p, obstacles)
    {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x() + fixCenter, itemp.y() + fixCenter, REAL_OBSTACLE_SIZE, REAL_OBSTACLE_SIZE));
    }
    return path;
}

void Obstacle::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->save();
    painter->fillPath(shape(), Qt::gray);
    painter->restore();
}

void Obstacle::change(const QPointF& pos)
{
    QPointF topLeft = topLeftPos(pos);
    if (!inMoveArea(topLeft))
        return;
    for (auto iter = obstacles.begin(); iter != obstacles.end(); iter++)
    {
        if (*iter == topLeft)
        {   // has this obstacle
            obstacles.erase(iter);
            return;
        }
    }
    // don't have this obstacle
    QPointF fixed(topLeft.x() + fixCenter, topLeft.y() + fixCenter);
    QGraphicsRectItem newObstacle(QRectF(mapToScene(fixed), QSizeF(REAL_OBSTACLE_SIZE, REAL_OBSTACLE_SIZE)));
    scene()->addItem(&newObstacle);
    auto list = scene()->items();
    bool collided = 0;
    foreach (auto p, list)
    {
        // qDebug() << newObstacle.collidesWithItem(p);
        if (&newObstacle != p && newObstacle.collidesWithItem(p))
        {
            collided = 1;
            break;
        }
    }
    if (!collided)
        obstacles.push_back(topLeft);
    /* WTF???
    auto collisions = newObstacle.collidingItems();
    if (collisions.isEmpty())
        obstacles.push_back(topLeft);
    */
    scene()->removeItem(&newObstacle);
}

QDataStream& operator>>(QDataStream& in, Obstacle& _obstacle)
{
    in >> _obstacle.obstacles;
    return in;
}

QDataStream& operator<<(QDataStream& out, const Obstacle& _obstacle)
{
    out << _obstacle.obstacles;
    return out;
}
