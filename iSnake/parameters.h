#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <QRandomGenerator>
#include <QList>
#include <QRectF>
#include <QPointF>
#include <QDebug>

const QString CURRENT_DIR   = "./";

const int FPS               = 40;
const int PERIOD            = 1000 / FPS;
const int MAP_SIZE          = 500;
const int WALL_SIZE         = 10;
const int WALL_COUNT        = MAP_SIZE / WALL_SIZE;
const int BLOCK_SIZE        = 12;
const int BLOCK_COUNT       = 40;
/* for "central use"
const int MAP_MOVE_SIZE     = MAP_SIZE - 2 * WALL_SIZE; // 480
const int MAP_MOVE_MININDEX = - BLOCK_COUNT / 2; // -20
const int MAP_MOVE_MAXINDEX = BLOCK_COUNT / 2 - 1;    // 19
const int MAP_MOVE_MINPOS   = - BLOCK_SIZE * (BLOCK_COUNT / 2); // -240
const int MAP_MOVE_MAXPOS   = BLOCK_SIZE * (BLOCK_SIZE / 2);    // 240
const int MAP_MINPOS        = - BLOCK_SIZE * (BLOCK_COUNT / 2) - WALL_SIZE; // -250
const int MAP_MAXPOS        = BLOCK_SIZE * (BLOCK_SIZE / 2);    // 250
*/

const int INIT_SNAKE_SPEED  = 15;

const int MAP_MOVE_SIZE     = MAP_SIZE - 2 * WALL_SIZE; // 480
const int MAP_MOVE_MININDEX = 0;
const int MAP_MOVE_MAXINDEX = BLOCK_COUNT - 1;  // 39
const int MAP_MOVE_MINPOS   = WALL_SIZE;    // 10
const int MAP_MOVE_MAXPOS   = MAP_MOVE_MINPOS + BLOCK_COUNT * BLOCK_SIZE;   // 490 - 12 = 478
const int MAP_MINPOS        = 0;
const int MAP_MAXPOS        = MAP_SIZE;

const qreal fixCenter       = 0.5;

inline int randomIndex()
{
    return (qrand() % BLOCK_COUNT);
}

inline int randomIndexNotEdge(const int left = 0, const int right = 0)
{
    int ret = 0;
    do
    {
        ret = randomIndex();
    } while (ret <= MAP_MOVE_MININDEX + left || ret == MAP_MOVE_MAXINDEX - right);
    return ret;
}

inline qreal randomMovePos()
{
    return MAP_MOVE_MINPOS + randomIndex() * BLOCK_SIZE;
}

inline qreal randomPosWithNotEdge(const int left = 0, const int right = 0)
{
    return randomIndexNotEdge(left, right) * BLOCK_SIZE;
}

inline bool inMoveArea(const qreal x)
{
    return MAP_MOVE_MINPOS <= x && x <= MAP_MOVE_MAXPOS;
}

inline bool inMoveArea(const qreal x, const qreal y)
{
    return inMoveArea(x) && inMoveArea(y);
}

inline bool inMoveArea(const QPointF p)
{
    return inMoveArea(p.x()) && inMoveArea(p.y());
}

QPointF topLeftPos(const QPointF& pos);

#endif // PARAMETERS_H
