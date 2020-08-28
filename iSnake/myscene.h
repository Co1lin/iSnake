#ifndef MYSCENE_H
#define MYSCENE_H

#include <QGraphicsScene>

class Game;

class MyScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit MyScene(QObject *parent = nullptr);
    void setGame(Game* _game)
    {
        game = _game;
    }

signals:
    void mousePressed(const QPointF& pos);

private:
    Game* game;

    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

};

#endif // MYSCENE_H
