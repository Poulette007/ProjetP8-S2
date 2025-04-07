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
#include "GameOver.h"

#include "const.h"
QGraphicsScene* gameScene = nullptr;
#include "ImageManager.h"
#include "ConnectionSerie.h"
#include <QStyleFactory>
#include <qprocess.h>

bool isRelance = false;

void relancerProgram()
{
	QString programPath = QCoreApplication::applicationFilePath();
	QStringList arguments = QCoreApplication::arguments();
    arguments << "--relance";  // argument fictif pour différencier

    QProcess::startDetached(programPath, arguments);
    QCoreApplication::exit(0);  // quitte le programme actuel
}


int main(int argc, char* argv[])
{
    //Set-Up
    QApplication app(argc, argv);
    QStringList args = QCoreApplication::arguments();
	if (args.contains("--relance"))
	{
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
	Pages->addWidget(GameOverPage);

    Stat* stat = new Stat();
    Game* game = new Game(stat, Pages, GameOverPage);

    //Timer pour update le jeu
    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() { game->update(); });

    //Timer pour lire le clavier
    QTimer readKeyTimer;
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->readKeyBoardGame(); });
    //QObject::connect(&readKeyTimer, &QTimer::timeout, [&]() { game->takeoff->readInputDecollage();});
    QObject::connect(&readKeyTimer, &QTimer::timeout, [&]()
        {
            if (game->state == Game::Gamestate::Landing)
            {
                game->landing->readInputAtterrissage();
            }

        });
    //Connect les bouttons pour changer de page
    QObject::connect(LoginPage->NextPage, &QPushButton::clicked, [&]() {
        if(LoginPage->ButtonPushed())
            Pages->setCurrentIndex(1);
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