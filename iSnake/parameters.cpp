#include "parameters.h"

QPointF topLeftPos(const QPointF& pos)
{
    QPointF topLeftPos;
    topLeftPos.rx() = WALL_SIZE + BLOCK_SIZE * (static_cast<int>(floor(floor(pos.x() - WALL_SIZE) / static_cast<double>(BLOCK_SIZE))));
    qDebug() << floor(pos.x() - WALL_SIZE) / static_cast<double>(BLOCK_SIZE);
    qDebug() << topLeftPos.rx();
    topLeftPos.ry() = WALL_SIZE + BLOCK_SIZE * (static_cast<int>(floor(floor(pos.y() - WALL_SIZE) / static_cast<double>(BLOCK_SIZE))));
    if (topLeftPos.rx() < 0)
        topLeftPos.rx() = 0;
    if (topLeftPos.ry() < 0)
        topLeftPos.ry() = 0;
    return topLeftPos;
}
