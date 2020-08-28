#include <QTime>
#include <QGraphicsTextItem>
#include <QEvent>
#include <QDebug>
#include <QObject>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>

#include "game.h"
#include "myscene.h"
#include "snake.h"
#include "food.h"
#include "wall.h"
#include "obstacle.h"

Game::Game(QSharedPointer<QGraphicsScene> _scene, MainWindow* _parent):
    parent(_parent), scene(_scene)
{
    welcome();
    snake = nullptr;
    food = nullptr;
    wall = nullptr;
    obstacle = nullptr;
    paused = -1;
}

void Game::welcome()
{
    scene->clear();
    auto text = new QGraphicsTextItem("Welcome to iSnake!\nPress Space to start.");
    text->setDefaultTextColor(Qt::white);
    scene->addItem(text);
}

void Game::start()
{
    paused = -1;    // waiting to start
    achivement = 0;
    emit stepsChanged(0);
    emit achivementChanged(achivement);
    qsrand(static_cast<uint>(QTime(0, 0, 0).secsTo(QTime::currentTime())));
    scene->clear();
    timer.start(PERIOD);
    // create the wall
    wall = new Wall();
    scene->addItem(wall);
    // new a snake
    snake = new Snake(this);
    connect(snake, &Snake::stepsChanged, [=](const int _step){ emit stepsChanged(_step); });
    emit snakeCreated();
    //qDebug() << "add a Snake: " << snake;
    scene->addItem(snake);
    // new a food
    addFood();
    // add the obstacle
    obstacle = new Obstacle();
    scene->addItem(obstacle);
    connect(qSharedPointerCast<MyScene>(scene).get(), &MyScene::mousePressed, this, &Game::changeObstacle);
    // install eventFilter for scene AFTER start the game
    scene->installEventFilter(this);
    connect(&timer, &QTimer::timeout, scene.get(), &QGraphicsScene::advance);
}

void Game::load(QDataStream& in)
{
    paused = 1;
    scene->clear();
    timer.start(PERIOD);
    // create the wall
    if (!wall)
    {
        wall = new Wall();
        scene->addItem(wall);
    }
    in >> achivement;
    emit achivementChanged(achivement);
    // recover the snake
    snake = new Snake(this, in);
    emit stepsChanged(snake->getSteps());
    emit snakeCreated();
    connect(snake, &Snake::stepsChanged, [=](const int _step){ emit stepsChanged(_step); });
    scene->addItem(snake);
    // recover the food
    food = new Food();
    in >> *food;
    scene->addItem(food);
    // recover the obstacle
    obstacle = new Obstacle();
    in >> *obstacle;
    scene->addItem(obstacle);
    scene->installEventFilter(this);
}

void Game::addFood()
{
    int x, y;
    food = nullptr;
    do
    {
        if (food)
        {
            scene->removeItem(food);
            delete food;
        }
        x = int(randomMovePos());
        y = int(randomMovePos());
        food = new Food(x, y);
        scene->addItem(food);
    } while (snake->collidesWithItem(food) || snake->collidesWithItem(obstacle));
    // scene->addItem(food);
    qDebug() << "add a Food: " << food << ": " << x << ", " << y;
}

void Game::changeObstacle(const QPointF& pos)
{
    if (paused == -1)
    {
        obstacle->change(pos);
    }
}

bool Game::eventFilter(QObject *watched, QEvent *event)
{
    if (event->type() == QEvent::KeyPress)
    {
        qDebug() << "KeyPress!\n";
        auto keyEvent = dynamic_cast<QKeyEvent*>(event);
        return handleKeyPressed(keyEvent) || QObject::eventFilter(watched, event);
    }
    else if (event->type() == QEvent::GraphicsSceneMousePress)
    {
        qDebug() << "clicked: ";
        return QObject::eventFilter(watched, event);
    }
    else
    {
        return QObject::eventFilter(watched, event);
    }
}

bool Game::handleKeyPressed(QKeyEvent* event)
{
    if (paused == 1)
    {   // when paused, only pressing cmd + R can resume
        return false;
    }
    else
    {
        switch (event->key())
        {
        case Qt::Key_Left:
            snake->setDirection(Snake::MoveLeft);
            return true;
        case Qt::Key_Right:
            snake->setDirection(Snake::MoveRight);
            return true;
        case Qt::Key_Up:
            snake->setDirection(Snake::MoveUp);
            return true;
        case Qt::Key_Down:
            snake->setDirection(Snake::MoveDown);
            return true;
        default:
            return false;
        }
    }
}

void Game::snakeDie()
{
    // call gameOver at the beginning of the last event loop
    QTimer::singleShot(0, this, &Game::gameOver);
}

void Game::pause()
{
    disconnect(&timer, &QTimer::timeout, scene.get(), &QGraphicsScene::advance);
    paused = 1;
}

void Game::resume()
{
    connect(&timer, &QTimer::timeout, scene.get(), &QGraphicsScene::advance);
    paused = 0;
}

void Game::restart()
{
    start();
}

void Game::gameOver()
{
    disconnect(&timer, &QTimer::timeout, scene.get(), &QGraphicsScene::advance);
    paused = 2;
    // show info
    auto overText = new QGraphicsTextItem("Game Over!");
    scene->addItem(overText);
    overText->setPos(MAP_SIZE / 2 - 4 * BLOCK_SIZE, MAP_SIZE / 2);
    emit gameOvered();
}

void Game::setSpeed(const int _speed)
{
    if (snake)
        snake->setSpeed(_speed);
}

void Game::snakeEatFood()
{
    emit achivementChanged(++achivement);
    scene->removeItem(food);
    addFood();
}
