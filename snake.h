#ifndef SNAKE_H
#define SNAKE_H

#include <QGraphicsItem>
#include <QSpinBox>

class Game;

class Snake : public QGraphicsObject
{
    Q_OBJECT
public:
    enum Direction
    {
        NoMove,
        MoveLeft,
        MoveRight,
        MoveUp,
        MoveDown,
        ForbidLeft,
        ForbidRight,
        ForbidUp,
        ForbidDown
    };

    Snake(Game* _game);
    Snake(Game* _game, QDataStream& in);

    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = nullptr) override;
    QPainterPath shape() const override;

    void setDirection(const Direction& _direction);
    Direction getDirection();

    int getSteps() const
    {
        return steps;
    }
    int getTimeCounter()
    {
        return timeCounter;
    }
    int getSpeed()
    {
        return speed;
    }

    friend QDataStream& operator<<(QDataStream& out, const Snake& _snake);
    friend QDataStream& operator>>(QDataStream& in, Snake& _snake);

signals:
    void stepsChanged(int);

public slots:
    void setSpeed(int _speed)
    {
        speed = _speed;
    }

protected:
    void advance(int phase) override;

private:
    Game* game;

    QList<QPointF> body;
    Direction direction;
    int growing;
    int speed;
    int steps;
    int timeCounter;

    QPointF moveLeft();
    QPointF moveRight();
    QPointF moveUp();
    QPointF moveDown();

    bool handleCollisions();

};

#endif // SNAKE_H
