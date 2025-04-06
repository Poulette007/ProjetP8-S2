#include <stdlib.h> 
#include <windows.h>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"
#include "Takeoff.h"
#include "Landing.h"
#include "MainMenu.h"
#include "login.h"

#include "const.h"
QGraphicsScene* gameScene = nullptr;
#include "ImageManager.h"
#include "ConnectionSerie.h"
#include <QStyleFactory>
#include <QStackedWidget>


int main(int argc, char* argv[])
{
    //Set-Up
    QApplication app(argc, argv);

    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0, 0, 1920, 1080);

    ConnectionSerie connection;

    Stat* stat = new Stat();
    Game* game = new Game(stat);

    //Timer pour update le jeu
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });

    //Timer pour lire le clavier
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

    //Creation du widget qui contien tous les autres
    QWidget Jeu;

    //Creation du stack de widget
    QStackedWidget* Pages = new QStackedWidget(&Jeu);

    //Creation de la page de login
    login* LoginPage = new login();

    //Creation de la page de menu
    MainMenu* MenuPage = new MainMenu();

    //Creation de la view
    QGraphicsView* view = new QGraphicsView(gameScene);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setContentsMargins(0, 0, 0, 0);
    view->setFrameStyle(QFrame::NoFrame);
    view->setBackgroundBrush(Qt::NoBrush);
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
    view->showFullScreen();
    //Ajout des pages dans le stack
    Pages->addWidget(LoginPage);
    Pages->addWidget(MenuPage);
    Pages->addWidget(view);

    //Connect les bouttons pour changer de page
    QObject::connect(LoginPage->NextPage, &QPushButton::clicked, [&]() {
        if(LoginPage->ButtonPushed())
            //Pages->setCurrentIndex(1);
            Pages->setCurrentIndex(2);
            timer.start(20 - stat->speedfactor);
            readKeyTimer.start(60);

        });
    QObject::connect(MenuPage->BackPage, &QPushButton::clicked, [&]() {
        Pages->setCurrentIndex(0);
        view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
        });
    QObject::connect(MenuPage->NextPage, &QPushButton::clicked, [&]() {
        Pages->setCurrentIndex(2);
        timer.start(20 - stat->speedfactor);
        readKeyTimer.start(60);

        });



    //Cree le layout pour notre fenetre
    QVBoxLayout* mainLayout = new QVBoxLayout(&Jeu);
    mainLayout->addWidget(Pages);
    Jeu.setLayout(mainLayout);
    //Jeu.resize(1920, 1080);
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
	//Jeu.setStyle(QStyleFactory::create("Fusion"));
    //Show et run!
    Jeu.showFullScreen();
    return app.exec();
}



/*#include <stdlib.h> 
#include <windows.h>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"

#include "const.h"
#include "login.h"
QGraphicsScene* gameScene = nullptr;
#include "ImageManager.h"
#include "ConnectionSerie.h"
#include <QStyleFactory>
int main(int argc, char* argv[])
{
    QApplication app(argc, argv);
    login *log = new login();
    log->show();
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

    Stat *stat = new Stat();
    Game* game = new Game(stat);

    //std::cout << "tete";
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });
    timer.start(20 - stat->speedfactor);

    //Timer pour lire le clavier
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
 }*/



