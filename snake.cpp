#include <QPainter>
#include <QDebug>
#include <QGraphicsScene>
#include "snake.h"
#include "parameters.h"
#include "game.h"
#include "food.h"
#include "wall.h"
#include "obstacle.h"

static const qreal SNAKE_SIZE = BLOCK_SIZE;

Snake::Snake(Game* _game): game(_game)
{
    timeCounter = 0;
    steps = 0;
    showHead = 0;
    emit stepsChanged(steps);
    // qDebug() << "emit!";
    QPointF head;
    QPointF secondBody;
    do
    {
        head = QPointF(randomMovePos(), randomMovePos());
        bool flag = qrand() % 2;
        int delta = (flag == 0 ? -1 : 1) * BLOCK_SIZE;
        if (qrand() % 2)
        {
            secondBody.setX(head.x() + delta);
            secondBody.setY(head.y());
        }
        else
        {
            secondBody.setX(head.x());
            secondBody.setY(head.y() + delta);
        }
    } while (!inMoveArea(secondBody.x(), secondBody.y()));

    this->setPos(head);
    body << head << secondBody;
    growing = 0;
    speed = INIT_SNAKE_SPEED;
    qDebug() << "New a Snake: " << this << ": " << head.x() << ", " << head.y() << endl;
    if (abs(secondBody.x() - head.x() + SNAKE_SIZE) <= 1e-6)
        direction = ForbidLeft;
    else if (abs(secondBody.x() - head.x() - SNAKE_SIZE) <= 1e-6)
        direction = ForbidRight;
    else if (abs(secondBody.y() - head.y() - SNAKE_SIZE) <= 1e-6)
        direction = ForbidDown;
    else if (abs(secondBody.y() - head.y() + SNAKE_SIZE) <= 1e-6)
        direction = ForbidUp;

}

QRectF Snake::boundingRect() const
{
    qreal minX = body.first().x();
    qreal minY = body.first().y();
    qreal maxX = body.first().x();
    qreal maxY = body.first().y();

    foreach (auto& p, body)
    {
        maxX = p.x() > maxX ? p.x() : maxX;
        maxY = p.y() > maxY ? p.y() : maxY;
        minX = p.x() < minX ? p.x() : minX;
        minY = p.y() < minY ? p.y() : minY;
    }

    QPointF tl = mapFromScene(QPointF(minX, minY));
    QPointF br = mapFromScene(QPointF(maxX, maxY));

    QRectF bound = QRectF(tl.x(),  // x
                          tl.y(),  // y
                          br.x() - tl.x() + SNAKE_SIZE, // width
                          br.y() - tl.y() + SNAKE_SIZE  // height
                          );
    return bound;
}

QPainterPath Snake::shape() const
{
    QPainterPath path;
    path.setFillRule(Qt::WindingFill);
    // 画头
    // path.addRect(QRectF(0, 0, SNAKE_SIZE, SNAKE_SIZE));
    // 画身
    foreach (auto& p, body)
    {
        QPointF itemp = mapFromScene(p);
        path.addRect(QRectF(itemp.x(), itemp.y(), SNAKE_SIZE, SNAKE_SIZE));
    }
    return path;
}

void Snake::paint(QPainter *painter, const QStyleOptionGraphicsItem*, QWidget*)
{
    painter->save();
    painter->fillPath(shape(), Qt::yellow);
    // painter->drawLine(0, 0, SNAKE_SIZE, SNAKE_SIZE);
    if (showHead)
    {
        painter->setPen(Qt::green);
        painter->drawEllipse(QPointF(SNAKE_SIZE / 2.0 + fixCenter, SNAKE_SIZE / 2.0 + fixCenter), SNAKE_SIZE / 2.6, SNAKE_SIZE / 2.6);
    }
    painter->restore();
}

void Snake::setDirection(const Direction& _direction)
{
//    if ((direction == MoveLeft || direction == ForbidRight) && _direction == MoveRight)
//        return;
//    if ((direction == MoveRight  || direction == ForbidLeft)&& _direction == MoveLeft)
//        return;
//    if ((direction == MoveUp || direction == ForbidDown) && _direction == MoveDown)
//        return;
//    if ((direction == MoveDown || direction == ForbidUp) && _direction == MoveUp)
//        return;
    if ((direction == MoveLeft && _direction == MoveRight) ||
        (direction == MoveRight && _direction == MoveRight) ||
        (direction == MoveUp && _direction == MoveDown) ||
         (direction == MoveDown && _direction == MoveUp))
        return;
    else if ((direction == ForbidRight && _direction == MoveRight) ||
             (direction == ForbidLeft && _direction == MoveLeft) ||
             (direction == ForbidUp && _direction == MoveUp) ||
             (direction == ForbidDown && _direction == MoveDown))
    {
        turnAround();
    }
    direction = _direction;
    game->setPausedState(0);    // enter nomal playing state if it's not
}

Snake::Direction Snake::getDirection()
{
    return direction;
}

void Snake::advance(int phase)
{
    if (phase ||
        timeCounter++ % (20 - speed + 1) != 0 ||
        direction == NoMove )
        return;
    else if (
        direction == ForbidRight ||
        direction == ForbidLeft ||
        direction == ForbidUp ||
        direction == ForbidDown )
    {
        return;
    }

    if (timeCounter >= 100000000)
        timeCounter = 0;
    if (growing > 0)
    {
        growing--;
    }
    else
    {
        body.removeLast();
    }
    switch(direction)
    {
    case MoveLeft:
        moveLeft(); // head will be pushed into the list
        break;
    case MoveRight:
        moveRight();
        break;
    case MoveUp:
        moveUp();
        break;
    case MoveDown:
        moveDown();
        break;
    default:
        ;
    }
    showHead = 1;
    setPos(body.first());
    if (handleCollisions() == 0)
        emit stepsChanged(++steps);
}

void Snake::turnAround()
{
    if (body.size() == 2)
    {
        qDebug() << "trun around!";
        QPointF tmp = body.takeFirst();
        body << tmp;
    }
}

QPointF Snake::moveLeft()
{
    QPointF head = body.first();
    head.rx() -= SNAKE_SIZE;
    body.push_front(head);
    return head;
}


QPointF Snake::moveRight()
{
    QPointF head = body.first();
    head.rx() += SNAKE_SIZE;
    body.push_front(head);
    return head;
}


QPointF Snake::moveUp()
{
    QPointF head = body.first();
    head.ry() -= SNAKE_SIZE;
    body.push_front(head);
    return head;
}

QPointF Snake::moveDown()
{
    QPointF head = body.first();
    head.ry() += SNAKE_SIZE;
    body.push_front(head);
    return head;
}

bool Snake::handleCollisions()
{
    {
        // collide with itself
        auto head = body.first();
        body.removeFirst();
        if (body.contains(head))
        {
            game->snakeDie();
            return 1;
        }
        else
            body.push_front(head);
    }

    {
        QGraphicsRectItem headItem(body.first().x(), body.first().y(), SNAKE_SIZE, SNAKE_SIZE);
        auto p = scene();
        // auto plist = p->items();
        p->addItem(&headItem);
        Food* food = game->getFood();
        // qDebug() << "here 01!\n";
//        auto realHeadPos = QGraphicsItem::mapToScene(headItem.pos());
//        qDebug() << "head: " << realHeadPos.x() << ", " << realHeadPos.y();
        if (food != nullptr && headItem.collidesWithItem(food))
        {   // collide with food, aka eat a food
            qDebug() << "Eat a food!\n";
            game->snakeEatFood();
            growing += 3;
            // when eat a food, the snake will neither collide with itself not collide with the wall
        }
        else if (headItem.collidesWithItem(game->getWall()))
        {   // collide with the wall
            game->snakeDie();
            return 1;
        }
        else if (headItem.collidesWithItem(game->getObstacle()))
        {   // collide with the obstacle
            game->snakeDie();
            return 1;
        }

        p->removeItem(&headItem);
    }
    return 0;
}

QDataStream& operator<<(QDataStream& out, const Snake& _snake)
{
    out << _snake.showHead << _snake.body << _snake.direction << _snake.growing << _snake.speed << _snake.steps << _snake.timeCounter;
    return out;
}

QDataStream& operator>>(QDataStream& in, Snake& _snake)
{
    int _direction;
    in >> _snake.showHead >> _snake.body >> _direction >> _snake.growing >> _snake.speed >> _snake.steps >> _snake.timeCounter;
    _snake.direction = Snake::Direction(_direction);
    _snake.setPos(_snake.body.first());
    return in;
}

Snake::Snake(Game* _game, QDataStream& in):
    game(_game)
{
    in >> *this;
}
