# iSnake

A classic game "snake", the coursework for Qt programming, in summer term 2020.  

[![GitHub license](https://img.shields.io/github/license/Co1lin/iSnake?style=flat-square)](https://github.com/Co1lin/iSnake/blob/master/LICENSE)  [![Qt](https://img.shields.io/badge/Qt-5-brightgreen?style=flat-square)](https://www.qt.io/)  [![C++](https://img.shields.io/badge/C%2B%2B-11-brightgreen?style=flat-square)](https://en.cppreference.com)  [![CodeFactor](https://www.codefactor.io/repository/github/co1lin/isnake/badge?style=flat-square)](https://www.codefactor.io/repository/github/co1lin/isnake)  [![GitHub stars](https://img.shields.io/github/stars/Co1lin/iSnake?style=flat-square)](https://github.com/Co1lin/iSnake/stargazers)  [![GitHub forks](https://img.shields.io/github/forks/Co1lin/iSnake?style=flat-square)](https://github.com/Co1lin/iSnake/network)  

![iSnakeDemo](https://github.com/Co1lin/iSnake/blob/master/others/logo.png)  

## Tips

The program for macOS is "iSnake.app", in "iSnake.app.zip".  

The file "logo.snake" is a saved game file, with the snake, food, and obstacles organized  in a special shape which reads "iSnake" as the picture above.  You can load this file in the game and play it!  

## Design Document

The main structure and logic of this program is shown by the picture below.  
(The solid bule arrow means a has b.)

![structure](https://github.com/Co1lin/iSnake/blob/master/others/structure.png)  

### MainWindow

MainWindow is a child class of QMainWindow, and its objects holds all of the items of the game. MainWindow controls the class "Game", which can be regarded as a game controller, and the game controller controls the scene.  

The items below are placed in nested grid layout. And they are set with proper "size policy" in Qt Designer. So users can change the windows' size, and the items will strech automatically.  

#### Actions (Tool Bar, Menu)

Actions, including start, pause, resume, restart, save, and load, are managed by MainWindow. They are created by Qt Designer, so they are automatically connnected to corresponding buttons' events on the tool bar, events of options in the menu, and their shortcuts.  

MainWindow use the slots of actions to respond the operation of player, and call class "Game",  to process the operations in detail.  

(Note: The icons of these actions are added by Qt Designer too, and they are saved in "qrc_Resources.cpp" in an array format.)  

#### Buttons

The buttons at the bottom are connected to corresponding actions, from their signals to actions' slots, when MainWindow is constructing.  
When an action is triggered, it will set the availability of these buttons in its slots. So the tool bar, menu, and buttons, are connected together.  

#### Widgets

There are some widgets on the right part.  

The horizontal slide and the spinbox are connected with each other, and both of them can change the speed of the snake, by calling related slot functions of class "Game".  

The LCD displayers, which shows the infomation of steps, used time (seconds), and achivement (how many food the snake have eaten), are connected to signals of class "Game". The other one, score, are caculated and updated at every step of the snake, which evaluates how much food the snake have eaten over a period of time at some speed. The score will be higher if the acivement is more, the used steps are less, and the moving speed is higher.

### class "Game"

Class "Game" process the main task of the program. It is derived from QObject, so it can both receive and emit signals.

It switches the status of game based on MainWindow's calling.

When a new game starts, it draws the map. It creates the Wall, the original Snake, the original Food, and the original Obstacle. The snake's position and pose are generated randomly. The Food's position is ramdom too, without colliding with snakes.

It holds on the main items:  
scene, snake, food, wall, obstacle  

It provided some functions to get the main items:  
getSnake, getFood, getWall, getObstacle  

It has an eventFilter, which is installed on the scene, and it distributes different events to corresponding items or functions.  

It has some slot functions:  
start, pause, resume, restart: recieve operations from MainWindow and change the status of the game.  
setSpeed, snakeDie, gameOver, snakeEatFood, changeObstable: process the event in the game.  

It has some signals:  
gameOvered, achievementChanged, stepsChanged, snakeCreated: report some of the infomations' changes to MainWindow.  

It has a timer, which emits the signal at a given frequency to call the scene's slot function "advance" which will push forward the process of game.  

### Gaphics View Framework

Graphics View Framework contains QGraphicsView, QGraphicsScene, and QGraphicsItem.  
Their relations are shown in the picture above.  
The main architecture of this program is based on the Graphics View Framework.  

#### MyScene

Class MyScene derives from QGraphicsObject, which has both the property of QGraphicsItem and QObject. The reason of choosing QGraphicsObject is that QGraphicsItem can not deal with slots and signals.  

MyScene overrides the "mousePressEvent". In the function, it will emit a signal to report the position of QGraphicsSceneMouseEvent. And in the game controller, the signal is connected to function changeObstacle. So an obstacle may be added by a mouse click.  

MyScene provides a "scene" for items. It is the basic function of QGraphicsScene. GGraphicItem like Snake, Food, Wall, and Obstacle, can be added to the scene, and the scene will manage them, display them, call the function "advance" which will call function of the same name "advance" of each items on it to push forward the scene. It also provides collision detecting function, so it's easy to know when the snake collides with food, obstacle, or wall.  

#### QGraphicsItem

As what is mentioned above, snake, food, obstable, and wall, are child class of QGraphicsItem. They are managed by MyScene.  

The key part of implementing QGraphicsItem's child class, or to say, defining a customized QGraphicsItem in order to manage some shapes by QGraphicsScene, is to implement three virtual functions of QGraphicsItem.  

The first is boundingRect, which returns a minimum rectangle that can cover the whole item.  
The second is paint, which paints the item with given QPainter pointer.  
The third is shape, which returns a QPainterPath that told a QPainter how to draw the item.  

##### Food

Food has a 2-D QPointF which shows its position.  

##### Wall

The wall is fixed and has no data. It shows as a pre-set shape.  

#### Obstacle

An Obstacle object has a QList consists of all positions of obstacles (obstacle bocks).  

##### Snake

The snake has a QList to store it's bodies.  

What's more, the snake needs to move. So it need a implementation of function "advance". In advance function, the snake will move forward according to the variable "direction". Because it will be called by scene's advance function, which will be triggered frequently by timer of game controller, finally it seems moving again and again.  

#### QGraphicsView

Just like we need a window to enjoy the scene outsite, QGraphicsView serves to view MyScene(QGraphicsScene).  

Held by MainWindow, QGraphicsView responds to a timer of MainWindow to keep the scene that it views at the centre of it and fit to it. It is done by function "fitInView".  

### QDataStream

The game status can be archived in a file, or reloaded to the program. It requires to save the data of items in the game. This program use QDataStream to implement this function.  

Just like std::ostream or std::instream, QDataStream can recieve or output some data types in Qt. In this program, class Snake, Food, Obstacle are provided with overloaded operator to interact with QDataSteam, both input and output.  

When action of saving is triggered, the slot function of MainWindow will create a QDataSteam object "out" bound with a QFile, and then "store" the data of snake, food, obstacle and other necessary parameters to "out". Finally, the data can be saved in a ".snake" file in binary form.  

The function of loading is similar with saving. It is just a reverse.  


## Usage

### Controlling

You can control the game by the **tool bar**, the **buttons** or **widgets** on the window, the **menu**, and the corresponding **shortcuts**. You can use them whenever they are enabled.  

Note:  
The tool bar can be placed at any edge of the window.  
You can drag and stretch the window, and the objects will stretch automatically. But if the game area is not close to a square, it will keep "vibrating" to put the whole area at the center. (This is an issue that need to be fixed.)  
You can **save** the game status to a file and **reload** it at another time.  

### Status

There are five status in this game.  

#### Waiting to start

Run the program, then it will **wait to start**.  
Press "Start" or the space bar to start a game.  

#### Beginning

After starting the game, you are at the **beginning**. There are a yellow snake of size two, a red food, and a circle of blue wall on the "map".   

Click on the empty space on the map, you can set grey **obstacles**. Only in this state can you set obstacles.  

#### Playing

Press direction keys to let the snake move towards any direction. At first the snake has no direction; or to say, the head and the tail are the same. When it starts moving according to your operation, A green circle will mark the head.  

The snake will die if it hit the wall, the obstacles, or itself.

You are supposed to eat more food within less steps and at higher speed to get a higher score.  

Note: You can use change the speed whenever you want.  

#### Pause

Press P to **pause**, and press R to resume.  

Note: In this state, you can restart the game or save the current status to a file.  

#### Game Over

Game will be over if the snake dies. Then you can restart or quit.  

