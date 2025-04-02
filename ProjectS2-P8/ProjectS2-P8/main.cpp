#include <stdlib.h> 
#include <windows.h>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"

#include "const.h"
QGraphicsScene* gameScene = nullptr;
#include "ImageManager.h"
#include "ConnectionSerie.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0, 0, 1920, 1080);

    QGraphicsView* view = new QGraphicsView(gameScene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setContentsMargins(0, 0, 0, 0);

    view->setFrameStyle(QFrame::NoFrame);
    view->setBackgroundBrush(Qt::NoBrush);
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
    view->showFullScreen();
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
    
    ConnectionSerie connection;
    ImageManager& imgManager = ImageManager::getInstance();
    QPixmap planeImg = imgManager.getImage(PLANE);


    Game* game = new Game();

    std::cout << "tete";
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });
    timer.start(16);

    QTimer readKeyTimer;
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->readKeyBoardGame(); });
    readKeyTimer.start(100);


    return app.exec();
 }



