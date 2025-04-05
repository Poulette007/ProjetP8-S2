#include "Landing.h"
#include "ImageManager.h"
#include "const.h"
Landing::Landing(Game* game, Plane* p, Stat* s, QGraphicsTextItem* prompt)
{
	initPiste();
	landingTimer = new QTimer(this);
	connect(landingTimer, &QTimer::timeout, this, &Landing::updateLanding);
	landingTimer->start(30);
	plane = p;
	stat = s;
	gameref = game;
	promptText = prompt;
	
}

void Landing::initPiste()
{
	longeurPiste = QRandomGenerator::global()->bounded(100, 300);
	//pour remplir le debut de la piste d'arbres
	for (int i = 0; i < longeurPiste / 4; i++)
	{
		QGraphicsPixmapItem* arbre = new QGraphicsPixmapItem();
		arbre->setPixmap(ImageManager::getInstance().getImage(TREE));
		arbre->setPos(i * arbre->pixmap().width()+1900, 1080 - arbre->pixmap().height());
		arbre->setZValue(0);	//arriere plan
		gameScene->addItem(arbre);
		runwayTilePixmap.push_back(arbre);
	}
	//remplissage du reste de la piste par des tuiles de piste
	for (int i = longeurPiste / 4; i < longeurPiste; i++)
	{
		QGraphicsPixmapItem* runwayTile = new QGraphicsPixmapItem();
		runwayTile->setPixmap(ImageManager::getInstance().getImage(RUNWAY));
		runwayTile->setScale(0.5);
		runwayTile->setPos(i * runwayTile->pixmap().width() / 2, 1080 - runwayTile->pixmap().height() / 2);
		runwayTile->setZValue(0);	//arriere plan
		gameScene->addItem(runwayTile);
		runwayTilePixmap.push_back(runwayTile);
	}
}

void Landing::updateLanding()
{
	int scrollSpeed = std::max(5, speed / 5);
	for (auto tile : runwayTilePixmap) {
		tile->setPos(tile->x() - scrollSpeed, tile->y());
	}
	switch (landingPhase)
	{
	case LandingPhase::Ralentissement:
		updateRalentissement();
		break;
	case LandingPhase::Descente:
		updateDescente();
		break;
	case LandingPhase::TrainAtterrissage:
		updateTrainAtterrissage();
		break;
	case LandingPhase::Atterissage:
		updateAtterrissage();
		break;
	case LandingPhase::Frein:
		updateFrein();
		break;
	case LandingPhase::Success:
		break;
	case LandingPhase::Failure:
		break;
	}
}

void Landing::updateRalentissement()
{
	//diminuer la vitesse de l'avion de 50%
	QString vitesse = QString::number(speed);
	
	if (plane->y() < 360)
	{
		promptText->setPlainText("Ralentir l'avion de 50%\nVitesse: " + vitesse);
		if (input == CLAVIER_A || input == BOUTON_GAUCHE)
		{
			speed -= 8;
			input = 0;
			qDebug() << "Vitesse : " << speed;
		}
		if (speed <= 50)
		{
			promptText->setPlainText("Ralentir l'avion de 50%\nVitesse: " + vitesse);
			landingPhase = LandingPhase::Descente;
			speed = 50;
			input = 0;
		}
	}
}
void Landing::updateDescente()
{
	//diminuer la hauteur de l'avion de 2 positions
	if(speed == 50)
	{
		if (plane->y() <= 1080 / 2)
		{
			if (input == CLAVIER_S || input == JOY_BAS)
			{
				plane->setY(plane->y() + 25);
				input = 0;
			}
		}
	}
}
void Landing::updateTrainAtterrissage()
{
}
void Landing::updateAtterrissage()
{
}
void Landing::updateFrein()
{
}
int Landing::readInputAtterrissage()
{
	if (GetAsyncKeyState('W') < 0)
	{
		input = CLAVIER_W;
	}
	if (GetAsyncKeyState('A') < 0)
	{
		input = CLAVIER_A;
	}
	if (GetAsyncKeyState('S') < 0)
	{
		input = CLAVIER_S;
	}
	if (GetAsyncKeyState('D') < 0)
	{
		input = CLAVIER_D;
	}
	if (GetAsyncKeyState(VK_SPACE) < 0)
	{
		input = SPACEBAR;
	}
	if (ConnectionSerie::hasData())
	{
		if (ConnectionSerie::getValue("BB") == 0)
		{
			input = BOUTON_BAS;
		}
		if (ConnectionSerie::getValue("BG") == 0)
		{
			input = BOUTON_GAUCHE;
		}
		if (ConnectionSerie::getValue("BD") == 0)
		{
			input = BOUTON_DROIT;
		}
		if (ConnectionSerie::getValue("BH") == 0)
		{
			input = BOUTON_HAUT;
		}
		if (ConnectionSerie::getValue("JB") == 0)
		{
			input = JOY_BAS;
		}
	}
	return input;
}