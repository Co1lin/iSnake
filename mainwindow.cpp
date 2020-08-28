#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QTimer>
#include <QDebug>
#include <QFileDialog>
#include <QMessageBox>
#include <QDateTime>
#include "parameters.h"
#include "game.h"
#include "myscene.h"
#include "snake.h"
#include "food.h"
#include "obstacle.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->spinBoxSpeed->setValue(INIT_SNAKE_SPEED);
    ui->horizontalSlider->setValue(INIT_SNAKE_SPEED);

    scene = QSharedPointer<MyScene>::create(this);
    ui->graphicsView->setScene(scene.get());
    game = QSharedPointer<Game>::create(scene, this);
    // initialize the scene
    //scene->setSceneRect(MAP_MINPOS, MAP_MINPOS, MAP_SIZE, MAP_SIZE);
    scene->setSceneRect(0, 0, MAP_SIZE, MAP_SIZE);

    // set speed
    connect(ui->horizontalSlider, &QSlider::valueChanged, ui->spinBoxSpeed, &QSpinBox::setValue);
    connect(ui->spinBoxSpeed, static_cast<void (QSpinBox:: *)(int)>(&QSpinBox::valueChanged), ui->horizontalSlider, &QSlider::setValue);
    connect(ui->actionSpeedUp, &QAction::triggered, [=]{
        qDebug() << "actionSpeedUp\n";
        ui->horizontalSlider->setValue(ui->horizontalSlider->value() + 1); });
    connect(ui->actionSpeedDown, &QAction::triggered, [=]{ ui->horizontalSlider->setValue(ui->horizontalSlider->value() - 1); });

    // set actions
    ui->actionPause->setEnabled(false);
    ui->actionResume->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->actionSave->setEnabled(false);

    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonResume->setEnabled(false);
    ui->pushButtonRestart->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);

    // set buttons
    connect(ui->pushButtonStart, &QPushButton::clicked, ui->actionStart, &QAction::trigger);
    connect(ui->pushButtonPause, &QPushButton::clicked, ui->actionPause, &QAction::trigger);
    connect(ui->pushButtonResume, &QPushButton::clicked, ui->actionResume, &QAction::trigger);
    connect(ui->pushButtonRestart, &QPushButton::clicked, ui->actionRestart, &QAction::trigger);
    connect(ui->pushButtonQuit, &QPushButton::clicked, ui->actionQuit, &QAction::trigger);
    connect(ui->pushButtonSave, &QPushButton::clicked, ui->actionSave, &QAction::trigger);
    connect(ui->pushButtonLoad, &QPushButton::clicked, ui->actionLoad, &QAction::trigger);
    // update window
    connect(&timerOfWindow, &QTimer::timeout, this, &MainWindow::update);
    timerOfWindow.start(PERIOD);

    connect(&timerShowTime, &QTimer::timeout, this, [=]{ ui->lcdNumberTime->display(++showTime); });

    connetWithGame();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::connetWithGame()
{
    scene->setGame(game.get());
    ui->lcdNumberSteps->display(0);
    ui->lcdNumberTime->display(0);
    ui->lcdNumberAchievement->display(0);
    ui->lcdNumberScore->display(0);
    connect(ui->horizontalSlider, &QSlider::valueChanged, game.get(), &Game::setSpeed);
    connect(game.get(), &Game::snakeCreated, [=]{ game->setSpeed(ui->horizontalSlider->value()); });
    connect(game.get(), &Game::achivementChanged, ui->lcdNumberAchievement, static_cast<void (QLCDNumber:: *)(int)>(&QLCDNumber::display));
    connect(game.get(), &Game::stepsChanged, ui->lcdNumberSteps, static_cast<void (QLCDNumber:: *)(int)>(&QLCDNumber::display));
    connect(game.get(), &Game::stepsChanged, [=]
    {
        int achievement = ui->lcdNumberAchievement->value();
        int steps = ui->lcdNumberSteps->value();
        if (steps == 0)
            ui->lcdNumberScore->display(0);
        else
            ui->lcdNumberScore->display(2000 * (static_cast<double>(achievement) / steps) * ui->spinBoxSpeed->value() / 15.0 );
    });
    showTime = 0;
    timerShowTime.stop();
    connect(game.get(), &Game::stepsChanged, this, [=](const int _step)
    {
        if (_step == 1)
        {
            timerShowTime.start(1000);
            // showTime++;
        }
    });
    connect(game.get(), &Game::gameOvered, this, &MainWindow::gameOvered);
    // connect(&timerShowTime, &QTimer::timeout, this, [=]{ ui->lcdNumberTime->display(++showTime); });
}

void MainWindow::update()
{
    // qDebug() << "update!\n";
    QRectF viewRect(scene->sceneRect().x(), scene->sceneRect().y(), scene->sceneRect().width() + 20, scene->sceneRect().height() + 20);
    ui->graphicsView->fitInView(viewRect, Qt::KeepAspectRatioByExpanding);
}

void MainWindow::on_actionStart_triggered()
{
    qDebug() << "start!\n";
    ui->actionStart->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->actionResume->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionPause->setEnabled(true);

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);
    ui->pushButtonResume->setEnabled(false);
    ui->pushButtonRestart->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonQuit->setEnabled(true);
    ui->pushButtonPause->setEnabled(true);

    game->start();
}

void MainWindow::on_actionPause_triggered()
{
    qDebug() << "pause!\n";
    timerShowTime.stop();
    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionResume->setEnabled(true);
    ui->actionRestart->setEnabled(true);
    ui->actionSave->setEnabled(true);

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);
    ui->pushButtonQuit->setEnabled(true);
    ui->pushButtonResume->setEnabled(true);
    ui->pushButtonRestart->setEnabled(true);
    ui->pushButtonSave->setEnabled(true);

    game->pause();
}

void MainWindow::on_actionResume_triggered()
{
    qDebug() << "resume!\n";
    timerShowTime.start(1000);
    ui->actionStart->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->actionResume->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionQuit->setEnabled(true);
    ui->actionPause->setEnabled(true);

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);
    ui->pushButtonResume->setEnabled(false);
    ui->pushButtonRestart->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonQuit->setEnabled(true);
    ui->pushButtonPause->setEnabled(true);

    game->resume();
}

void MainWindow::on_actionRestart_triggered()
{
    qDebug() << "restart!\n";

    ui->actionStart->setEnabled(true);
    ui->actionLoad->setEnabled(true);
    ui->actionQuit->setEnabled(true);
    ui->actionPause->setEnabled(false);
    ui->actionResume->setEnabled(false);
    ui->actionRestart->setEnabled(false);
    ui->actionSave->setEnabled(false);

    ui->pushButtonStart->setEnabled(true);
    ui->pushButtonLoad->setEnabled(true);
    ui->pushButtonQuit->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonResume->setEnabled(false);
    ui->pushButtonRestart->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);

    game.clear();
    game = QSharedPointer<Game>::create(scene, this);
    scene->setGame(game.get());
    connetWithGame();
}

void MainWindow::gameOvered()
{
    timerShowTime.stop();
    ui->actionStart->setEnabled(false);
    ui->actionPause->setEnabled(false);
    ui->actionResume->setEnabled(false);
    ui->actionLoad->setEnabled(false);
    ui->actionSave->setEnabled(false);
    ui->actionRestart->setEnabled(true);
    ui->actionQuit->setEnabled(true);

    ui->pushButtonStart->setEnabled(false);
    ui->pushButtonPause->setEnabled(false);
    ui->pushButtonResume->setEnabled(false);
    ui->pushButtonLoad->setEnabled(false);
    ui->pushButtonSave->setEnabled(false);
    ui->pushButtonRestart->setEnabled(true);
    ui->pushButtonQuit->setEnabled(true);
}

void MainWindow::on_actionQuit_triggered()
{
    exit(0);
}


void MainWindow::on_actionSave_triggered()
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save as..."), CURRENT_DIR + currentTime.toString() + ".snake", tr("Snake Files (*.snake)"));
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
        QMessageBox::warning(this, "Error!", "Can't save file: ", file.errorString());
    }
    else
    {
        QDataStream out(&file);
        // for file format checking
        out << static_cast<quint32>(0xA0B0C0D0);
        out << static_cast<quint32>(5);
        out.setVersion(QDataStream::Qt_5_13);

        out << showTime;
        out << game->getSnake()->getSpeed();
        out << game->getAchievement();
        out << *(game->getSnake());
        out << *(game->getFood());
        out << *(game->getObstacle());
        file.close();
    }
}

void MainWindow::on_actionLoad_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open the file...", CURRENT_DIR, tr("Snake Files (*snake)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(this, "Error!", "Can't open file: " + file.errorString());
    }
    else
    {
        qDebug() << "load!\n";

        QDataStream in(&file);
        // chech data format
        quint32 magic;
        in >> magic;
        if (magic != 0xA0B0C0D0)
        {
            QMessageBox::warning(this, "Error!", "File format is not right!");
            return;
        }
        in >> magic;
        if (magic == 5)
        {
            in.setVersion(QDataStream::Qt_5_13);
        }
        else
        {
            QMessageBox::warning(this, "Error!", "Version of file format is not right!");
            return;
        }
        ui->actionStart->setEnabled(false);
        ui->actionPause->setEnabled(false);
        ui->actionLoad->setEnabled(false);
        ui->actionQuit->setEnabled(true);
        ui->actionResume->setEnabled(true);
        ui->actionRestart->setEnabled(true);
        ui->actionSave->setEnabled(true);

        ui->pushButtonStart->setEnabled(false);
        ui->pushButtonPause->setEnabled(false);
        ui->pushButtonLoad->setEnabled(false);
        ui->pushButtonQuit->setEnabled(true);
        ui->pushButtonResume->setEnabled(true);
        ui->pushButtonRestart->setEnabled(true);
        ui->pushButtonSave->setEnabled(true);

        int speed;
        in >> showTime;
        ui->lcdNumberTime->display(showTime);
        in >> speed;
        ui->horizontalSlider->setValue(speed);
        game->load(in);
    }
}
