﻿#include <stdlib.h> 
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
#include "GameOver.h"

#include "const.h"
QGraphicsScene* gameScene = nullptr;
QGraphicsPixmapItem* BackGroundVol = nullptr;
#include "ImageManager.h"
#include "ConnectionSerie.h"
#include <QStyleFactory>
#include <qprocess.h>

bool isRelance = false;

void relancerProgram()
{
    QString programPath = QCoreApplication::applicationFilePath();
    QStringList arguments;
    
    arguments << "--relance" << Stat::playerName << QString::number(Stat::previousScore)
        << QString::number(login::SkinChecked);  // argument lors du redemanrrage de l'application

    QProcess::startDetached(programPath, arguments);

    ConnectionSerie::closeSerial();

    QCoreApplication::exit(0);  // quitte le programme actuel
}


int main(int argc, char* argv[])
{
    //Set-Up
    QApplication app(argc, argv);
    QStringList args = QCoreApplication::arguments();
	if (args.contains("--relance"))
	{
        Stat::playerName = args[2];
        Stat::previousScore = args[3].toInt();
        login::SkinChecked = args[4].toInt();
        Stat::skinPlane = (args[4].toInt()-1);
		isRelance = true;
	}
	else
	{
		isRelance = false;
	}

    gameScene = new QGraphicsScene();
    gameScene->setSceneRect(0, 0, 1920, 1080);

    ConnectionSerie connection;
    //Creation du widget qui contien tous les autres
    QWidget Jeu;

    //Creation du stack de widget
    QStackedWidget* Pages = new QStackedWidget(&Jeu);

    //Creation de la page de login
    login* LoginPage = new login();

    //page de Gameover
    GameOver* GameOverPage = new GameOver(false);

    //Creation de la page de menu
    MainMenu* MenuPage = new MainMenu();

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
    Pages->addWidget(GameOverPage);

    Stat* stat = new Stat();
    Game* game = new Game(stat, Pages, GameOverPage);


    //Create QTimer :
    QTimer timer;
    QTimer readKeyTimer;
    QTimer GenereObstacle;
	QTimer MuonTimer;


    // Game
	QObject::connect(&MuonTimer, &QTimer::timeout, [&]() {
		if (game->state == Game::Gamestate::Gameplay) {
            if (ConnectionSerie::getValue("Muon") == 1)
            {
                stat->muonTrue = true;
                if (stat->muonTrue)
                {
                    game->listActor.push_back(new Tree(1920, 1080 * 3 / 4));
                    gameScene->addItem(game->listActor.back());
                    stat->muonTrue = false;
                }
            }
		}});
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        if (game->state == Game::Gamestate::Gameplay) {
            game->update();
    }});
    QObject::connect(&GenereObstacle, &QTimer::timeout, [&]() {
        if (game->state == Game::Gamestate::Gameplay) {
            game->generateObstacles();
    }});
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() {
        if (game->state == Game::Gamestate::Gameplay) {
            game->readKeyBoardGame();
    }});
    // Decollage
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { 
        if (game->state == Game::Gamestate::Decollage) {
            game->takeoff->readInputDecollage();
            //GenereObstacle.stop();
    }});
	// Atterrissage
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() {
        if (game->state == Game::Gamestate::Landing) {
            game->landing->readInputAtterrissage();
            //GenereObstacle.stop();

    }});
    
   
    // Connect les bouttons pour changer de page
    // Start et stop les timers 
    QObject::connect(LoginPage->NextPage, &QPushButton::clicked, [&]() {
        if (LoginPage->ButtonPushed())
        {
           Pages->setCurrentIndex(1);
        }
		    
        });
    QObject::connect(MenuPage->BackPage, &QPushButton::clicked, [&]() {
		login::SkinChecked = 0;
        LoginPage->Plane->setChecked(false);
        LoginPage->Chopper->setChecked(false);
        LoginPage->Jet->setChecked(false);
		LoginPage->NewPlayer->setChecked(false);
		LoginPage->PlayerIsNew = false;
        Stat::previousScore = 0;
        Pages->setCurrentIndex(0);
        view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
        });
    QObject::connect(MenuPage->NextPage, &QPushButton::clicked, [&]() {
		stat->changeFuel(60);
        Pages->setCurrentIndex(2);
        game->changePlanePixmap();
        timer.start(16 - stat->speedfactor);
        readKeyTimer.start(80);
	    GenereObstacle.start(2000);
		MuonTimer.start(500);
    });
    QObject::connect(GameOverPage->Retour, &QPushButton::clicked, [&]() {
		relancerProgram();
        });
	


    //Cree le layout pour notre fenetre
    QVBoxLayout* mainLayout = new QVBoxLayout(&Jeu);
    mainLayout->addWidget(Pages);
    Jeu.setLayout(mainLayout);
    //Jeu.resize(1920, 1080);
    view->fitInView(gameScene->sceneRect(), Qt::KeepAspectRatio);
	//Jeu.setStyle(QStyleFactory::create("Fusion"));
    //Show et run!
    if (isRelance)
    {
        Pages->setCurrentIndex(1);
    }
    else
    {
		Pages->setCurrentIndex(0);
    }
    Jeu.showFullScreen();
    return app.exec();
}