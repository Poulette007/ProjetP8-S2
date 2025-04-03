#include <stdlib.h> 
#include <windows.h>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"
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

    ImageManager& imgManager = ImageManager::getInstance();
    QPixmap planeImg = imgManager.getImage(PLANE);

    Stat* stat = new Stat();
    Game* game = new Game(stat);

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
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);


    //Ajout des pages dans le stack
    Pages->addWidget(LoginPage);
    Pages->addWidget(MenuPage);
    Pages->addWidget(view);

    //Connect les bouttons pour changer de page
    QObject::connect(LoginPage->NextPage, &QPushButton::clicked, [Pages]() {
        Pages->setCurrentIndex(1);
        });
    QObject::connect(MenuPage->BackPage, &QPushButton::clicked, [Pages]() {
        Pages->setCurrentIndex(0);
        });
    QObject::connect(MenuPage->NextPage, &QPushButton::clicked, [Pages]() {
        Pages->setCurrentIndex(2);
        });

    //Cree le layout pour notre fenetre
    QVBoxLayout* mainLayout = new QVBoxLayout(&Jeu);
    mainLayout->addWidget(Pages);
    Jeu.setLayout(mainLayout);
    Jeu.resize(1920, 1080);
   
    //Timer pour update le jeu
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });
    timer.start(20 - stat->speedfactor);

    //Timer pour lire le clavier
    QTimer readKeyTimer;
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->readKeyBoardGame(); });
    readKeyTimer.start(100);
    
    //Show et run!
    Jeu.show();
    return app.exec();
}



