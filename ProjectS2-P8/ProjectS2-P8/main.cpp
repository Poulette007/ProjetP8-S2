#include <stdlib.h> 
#include <windows.h>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"
#include "Takeoff.h"
#include "Landing.h"
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

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });
    timer.start(100);

    QTimer readKeyTimer;
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->readKeyBoardGame(); });
	QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->takeoff->readInputDecollage();});
	QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() 
    {
		if (game->state == Game::Gamestate::Landing)
        {
            game->landing->readInputAtterrissage();
        }
        
    });
    readKeyTimer.start(60);


    return app.exec();
 }



