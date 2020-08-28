#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include "myscene.h"
#include "game.h"

MyScene::MyScene(QObject *parent) : QGraphicsScene(parent)
{
    game = nullptr;
}

void MyScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pos = event->scenePos();
    qDebug() << pos.x() << ", " << pos.y() << endl;
    emit mousePressed(pos);
}
