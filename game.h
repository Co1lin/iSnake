#ifndef GAME_H
#define GAME_H

#include <QObject>
#include <QGraphicsScene>
#include <QTimer>
#include <QSpinBox>

#include "mainwindow.h"

class MyScene;
// QGraphicsItem
class Snake;
class Food;
class Wall;
class Obstacle;

class Game : public QObject
{
    Q_OBJECT
public:
    Game(QSharedPointer<QGraphicsScene> scene, MainWindow* _parent);

    Snake* getSnake() const
    {
        return snake;
    }
    Food* getFood() const
    {
        return food;
    }
    Wall* getWall() const
    {
        return wall;
    }
    Obstacle* getObstacle() const
    {
        return obstacle;
    }
    int getPausedState() const
    {
        return paused;
    }
    void setPausedState(const int _state)
    {
        paused = _state;
    }
    int getAchievement() const
    {
        return achivement;
    }

    void load(QDataStream& in);

public slots:
    void start();
    void pause();
    void resume();
    void restart();
//    void exit();
//    void save();
//    void load();

    void setSpeed(const int _speed);
    void snakeDie();
    void gameOver();
    void snakeEatFood();
    void changeObstacle(const QPointF& pos);

signals:
    void gameOvered();
    void achivementChanged(int);
    void stepsChanged(int);
    void snakeCreated();

protected:
    bool eventFilter(QObject *watched, QEvent *event) override;

private:
    QObject* parent;

    QSharedPointer<QGraphicsScene> scene;
    QTimer timer;

    Snake* snake;
    Food* food;
    Wall* wall;
    Obstacle* obstacle;

    /*
     * -1: wait for start
     *  0: normal
     *  1: paused
     *  2: over
     */
    int paused;

    int achivement;

    void welcome();
    void addFood();
    void removeFood();

    bool handleKeyPressed(QKeyEvent* event);
    // bool handleMousePressed();
};

#endif // GAME_H
