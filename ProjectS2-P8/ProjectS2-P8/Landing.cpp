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
	for (int i = 0; i < longeurPiste / 8; i++)
	{
		QGraphicsPixmapItem* arbre = new QGraphicsPixmapItem();
		arbre->setPixmap(ImageManager::getInstance().getImage(TREE));
		arbre->setPos(i * arbre->pixmap().width()+1900, 1080 - arbre->pixmap().height());
		arbre->setZValue(0);	//arriere plan
		gameScene->addItem(arbre);
		runwayTilePixmap.push_back(arbre);
	}
	//remplissage du reste de la piste par des tuiles de piste
	for (int i = longeurPiste / 8; i < longeurPiste; i++)
	{
		QGraphicsPixmapItem* runwayTile = new QGraphicsPixmapItem();
		runwayTile->setPixmap(ImageManager::getInstance().getImage(RUNWAY));
		runwayTile->setScale(0.65);
		runwayTile->setPos(i * runwayTile->pixmap().width() / 2, 1080 - (runwayTile->pixmap().height() / 2) - 60);
		runwayTile->setZValue(0);	//arriere plan
		gameScene->addItem(runwayTile);
		runwayTilePixmap.push_back(runwayTile);
	}
}

void Landing::updateLanding()
{
	QGraphicsPixmapItem* lastTile = runwayTilePixmap.back();
	if (lastTile->x() + lastTile->pixmap().width() * lastTile->scale() < plane->x()) {
		landingPhase = LandingPhase::Failure;
		qDebug() << "you crashed.";
	}
	int scrollSpeed = std::max(5, speed / 5);
	for (auto tile : runwayTilePixmap) {
		tile->setPos(tile->x() - scrollSpeed, tile->y());
	}
	promptText->setPlainText("");
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
	
	if (speed > 50)
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
			stat->setSpeed(stat->getSpeed() / 2 + 1);
			speed = 50;
			input = 0;
		}
	}
	//diminuer la vitesse de 25%
	else if (speed > 25)
	{
		promptText->setPlainText("Ralentir l'avion de 25%\nVitesse: " + vitesse);
		if (input == CLAVIER_A || input == BOUTON_GAUCHE)
		{
			speed -= 5;
			input = 0;
			qDebug() << "Vitesse : " << speed;
		}
		if (speed <= 25)
		{
			promptText->setPlainText("Ralentir l'avion de 25%\nVitesse: " + vitesse);
			landingPhase = LandingPhase::Descente;
			speed = 25;
			stat->setSpeed(stat->getSpeed() / 2 + 1);
			input = 0;
		}
	}
	//au sol, diminuer la vitesse de 10%
	else if (speed > 10)
	{
		promptText->setPlainText("Ralentir l'avion de 10%\nVitesse: " + vitesse);
		if (input == CLAVIER_A || input == BOUTON_GAUCHE)
		{
			speed -= 2;
			input = 0;
			qDebug() << "Vitesse : " << speed;
		}
		if (speed <= 10)
		{
			promptText->setPlainText("Ralentir l'avion de 10%\nVitesse: " + vitesse);
			landingPhase = LandingPhase::Descente;
			stat->setSpeed(stat->getSpeed() / 4 + 1);
			speed = 10;
			input = 0;
		}
	}
}
void Landing::updateDescente()
{
	QString hauteur = QString::number(plane->y());
	//diminuer la hauteur de l'avion de 2 positions
	if(plane->y() < (1080 - plane->pixmap().height()) / 2 )
	{
		promptText->setPlainText("Descendre de moitié. Hauteur: " + QString::number(speed));
		if (input == CLAVIER_S || input == JOY_BAS)
		{
			plane->setY(plane->y() + 25);
			input = 0;
		}
		//reussite
		if (plane->y() >= (1080 - plane->pixmap().height()) / 2 )
		{
			promptText->setPlainText("Descendre de moitié. Hauteur: " + QString::number(speed));
			landingPhase = LandingPhase::Ralentissement;
			input = 0;
		}
	}
	else if (plane->y() < 800 - plane->pixmap().height() / 2)
	{
		promptText->setPlainText("Descendre a 10%. Hauteur: " + QString::number(speed));
		if (input == CLAVIER_S || input == JOY_BAS)
		{
			plane->setY(plane->y() + 25);
			input = 0;
		}
		//reussite
		if (plane->y() >= 800 - plane->pixmap().height() / 2)
		{
			promptText->setPlainText("Descendre a 10%: " + QString::number(speed));
			landingPhase = LandingPhase::TrainAtterrissage;
			input = 0;
		}
	}
	//toucher le sol hauteur : 1080 - plane.height - 10
	else if (plane->y() < 1080 - plane->pixmap().height() - 10)
	{
		promptText->setPlainText("Toucher le sol");
			if (input == CLAVIER_S || input == JOY_BAS)
			{
				plane->setY(plane->y() + 10);
				input = 0;
			}
		//toucher le sol good
		if (plane->y() >= 1080 - plane->pixmap().height() - 10)
		{
			promptText->setPlainText("Toucher le sol: reussite");
			landingPhase = LandingPhase::Atterissage;
			input = 0;
		}
	}
}
void Landing::updateTrainAtterrissage()
{
	promptText->setPlainText("Sortir le train d'atterrissage, W ou haut sur manette");
	if (input == CLAVIER_W || input == BOUTON_HAUT)
	{
		promptText->setPlainText("Sortir le train d'atterrissage, W ou haut sur manette : OK");
		landingPhase = LandingPhase::Ralentissement;
		input = 0;
		trainSorti = true;
	}
}
void Landing::updateAtterrissage()
{
	QString vitesse = QString::number(speed);
	promptText->setPlainText("imobilisez l'avion avec les freins. A A ou droite sur manette, Vitesse:" + vitesse);
	if (input == CLAVIER_A || input == BOUTON_GAUCHE)
	{
		if (speed > 0)
		{
			speed -= 1;
			input = 0;
		}
		else if (speed <= 0)
		{
			speed = 0;
			stat->setSpeed(0);
			promptText->setPlainText("imobilisez l'avion avec les freins, A ou droite sur manette : OK");
			landingPhase = LandingPhase::Success;
			input = 0;
		}
	}
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