#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>

#include "parameters.h"

class MyScene;
class Game;

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Ui::MainWindow* getUi()
    {
        return ui;
    }

private slots:
    void on_actionStart_triggered();
    void update();

    void on_actionPause_triggered();

    void on_actionQuit_triggered()__attribute__((noreturn));

    void on_actionResume_triggered();

    void on_actionRestart_triggered();

    void gameOvered();

    void on_actionSave_triggered();

    void on_actionLoad_triggered();

private:
    Ui::MainWindow *ui;
    QTimer timerOfWindow;
    QTimer timerShowTime;
    int showTime;

    // QGraphicsScene *scene;
    // QGraphicsView *view;
    QSharedPointer<MyScene> scene;
    QSharedPointer<QGraphicsView> view;

    QSharedPointer<Game> game;

    void connetWithGame();
    void welcome();

};
#endif // MAINWINDOW_H
