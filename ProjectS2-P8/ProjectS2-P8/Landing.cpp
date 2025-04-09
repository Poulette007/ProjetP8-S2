#define NOMINMAX
#include "const.h"
#include "ImageManager.h"
#include "Landing.h"
#include <algorithm>

Landing::Landing(Game* game, Plane* p, Stat* s, QGraphicsTextItem* prompt, QStackedWidget* stack, GameOver* gameOverPage)
{
	initPiste();
	landingTimer = new QTimer(this);
	connect(landingTimer, &QTimer::timeout, this, &Landing::updateLanding);
	landingTimer->start(30);
	plane = p;
	stat = s;
	this->stack = stack;
	gameref = game;
	promptText = prompt;
	this->gameOver = gameOverPage;
	promptText->setPlainText("");
	promptText->setPos(500, 100);
}

void Landing::initPiste()
{
	longeurPiste = QRandomGenerator::global()->bounded(100, 225);
	BackGroundVol->setPixmap(QPixmap(ImageManager::getInstance().getImage(BACKGROUND_ATTERRISSAGE_DECOLAGE)));
	//remplissage du reste de la piste par des tuiles de piste
	int i = 0;
	for (i = 0; i < longeurPiste / 8; i++)
	{
		QGraphicsPixmapItem* arbre = new QGraphicsPixmapItem();
		arbre->setPixmap(ImageManager::getInstance().getImage(TREE));
		arbre->setScale(1.5);
		arbre->setPos(i * arbre->pixmap().width() / 2 - 40, 1080 - (arbre->pixmap().height() / 2) - 60);
		arbre->setZValue(1);	//arriere plan
		gameScene->addItem(arbre);
		runwayTilePixmap.push_back(arbre);
	}
	for (i = longeurPiste / 8; i < longeurPiste; i++)
	{
		QGraphicsPixmapItem* runwayTile = new QGraphicsPixmapItem();
		runwayTile->setPixmap(ImageManager::getInstance().getImage(RUNWAY));
		runwayTile->setScale(0.65);
		runwayTile->setPos(i * runwayTile->pixmap().width() / 2, 1080 - (runwayTile->pixmap().height() / 2) - 60);
		runwayTile->setZValue(0);	//arriere plan
		gameScene->addItem(runwayTile);
		runwayTilePixmap.push_back(runwayTile);
	}
	//ajout une barriere de fin de piste
	QGraphicsPixmapItem* barriere = new QGraphicsPixmapItem();
	barriere->setPixmap(ImageManager::getInstance().getImage(BARRIERE));
	barriere->setScale(1.5);
	barriere->setPos(i * barriere->pixmap().width() / 2 + (1080 / 5), 1080 - (barriere->pixmap().height() / 2) - 250);
	barriere->setZValue(1);
	gameScene->addItem(barriere);
	runwayTilePixmap.push_back(barriere);
}

void Landing::updateLanding()
{
	QGraphicsPixmapItem* lastTile = runwayTilePixmap.back();
	if (lastTile->x() + lastTile->pixmap().width() * lastTile->scale() < plane->x()) {
		landingPhase = LandingPhase::Failure;
		qDebug() << "you crashed.";
	}
	int scrollSpeed = std::max(5, speed / 5);
	if (!frein)
	{
		for (auto tile : runwayTilePixmap) {
			tile->setPos(tile->x() - scrollSpeed, tile->y());
		}
	}
	gameref->afficherStat();
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
		gameref->state = Game::Gamestate::GameOver;
		gameOver->setVictoire(true);
		stack->setCurrentWidget(gameOver);
		saveScore();
		break;
	case LandingPhase::Failure:
		gameref->etteinManette();
		gameref->state = Game::Gamestate::GameOver;
		gameOver->setVictoire(false);
		stack->setCurrentWidget(gameOver);
		break;
	}
}
void Landing::updateRalentissement()
{
	//diminuer la vitesse de l'avion de 50%
	QString vitesse = QString::number(speed);
	int pot = abs((ConnectionSerie::getValue("pot")));
	pot = (pot / 7) % 100; // Convertit la valeur en pourcentage
	if (pot == 0)
	{
		pot = lastPot;
		return;
	}
	lastPot = pot;
	qDebug() << "pot: " << pot;
	if (speed > 50)
	{
		if (pot > 10)
		{
			speed = pot;
			stat->setSpeed(pot+1);
		}
		promptText->setPlainText("Ralentir l'avion a 50\nVitesse: " + vitesse+ "/100");
		if (speed <= 50)
		{
			promptText->setPlainText("Ralentir l'avion de 50\nVitesse: " + vitesse +"/100");
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
		speed = pot;
		qDebug() << "Vitesse : " << speed;
		if (speed <= 25)
		{
			promptText->setPlainText("Ralentir l'avion de 25%\nVitesse: " + vitesse);
			landingPhase = LandingPhase::Descente;
			speed = 25;
			stat->setSpeed(stat->getSpeed() / 4 + 1);
			input = 0;
		}
	}
	//au sol, diminuer la vitesse de 10%
	else if (speed > 10)
	{
		promptText->setPlainText("Ralentir l'avion de 10%\nVitesse: " + vitesse);
		speed = pot;
		qDebug() << "Vitesse : " << speed;
		if (speed <= 10)
		{
			promptText->setPlainText("Ralentir l'avion de 10%\nVitesse: " + vitesse);
			landingPhase = LandingPhase::Descente;
			stat->setSpeed(stat->getSpeed() / 10 + 1);
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
		promptText->setPlainText("Diminuez l'altitude a 540m\nAltitude: " + QString::number(1080-plane->y()-(plane->pixmap().height())/2));
		if (input == CLAVIER_S || input == JOY_BAS)
		{
			plane->setY(plane->y() + 35);
			input = 0;
		}
		//reussite
		if (plane->y() >= (1080 - plane->pixmap().height()) / 2 )
		{
			promptText->setPlainText("Diminuez l'altitude a 540m\nAltitude: " + QString::number(1080 - plane->y()-plane->pixmap().height()));
			landingPhase = LandingPhase::Ralentissement;
			input = 0;
		}
	}
	else if (plane->y() < 800 - plane->pixmap().height() / 2)
	{
		promptText->setPlainText("Diminuez l'altitude a 108m\nAltitude: " + QString::number(1080 - plane->y() - plane->pixmap().height()));
		if (input == CLAVIER_S || input == JOY_BAS)
		{
			plane->setY(plane->y() + 35);
			input = 0;
		}
		//reussite
		if (plane->y() >= 800 - plane->pixmap().height() / 2)
		{
			promptText->setPlainText("Diminuez l'altitude a 108m\nAltitude: " + QString::number(1080 - plane->y() - plane->pixmap().height()));
			landingPhase = LandingPhase::TrainAtterrissage;
			input = 0;
		}
	}
	//toucher le sol hauteur : 1080 - plane.height - 10
	else if (plane->y() < 1080 - plane->pixmap().height() - 10)
	{
		promptText->setPlainText("Touchez le sol");
			if (input == CLAVIER_S || input == JOY_BAS)
			{
				plane->setY(plane->y() + 25);
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
	promptText->setPlainText("Sortir le train d'atterrissage\nAppuyez sur le triangle");
	if (input == CLAVIER_W || input == BOUTON_HAUT)
	{
		promptText->setPlainText("Sortir le train d'atterrissage\nAppuyez sur le triangle: OK");
		landingPhase = LandingPhase::Ralentissement;
		input = 0;
		trainSorti = true;
	}
}
void Landing::updateAtterrissage()
{
	QString vitesse = QString::number(speed);
	promptText->setPlainText("Immobilisez l'avion avec les freins\nAppuyez sur le Cercle\nVitesse:" + vitesse + "Km/h");
	if ((input == CLAVIER_D || input == BOUTON_DROIT) && !frein)
	{
		frein = true;
		speed = 250;
	}
	if (frein)
	{
		if (speed >= 125)
		{
			plane->setX(plane->x() + 5);
			speed -= 1;
		}
		else if (speed < 125 && speed >50)
		{
			plane->setX(plane->x() + 3);
			speed -= 1;
		}
		else if (speed <= 50 && speed >0)
		{
			plane->setX(plane->x() + 2);
			speed -= 1;
		}
		else if (speed <= 0)
		{
			promptText->setPlainText("Atterrissage reussi!");
			landingPhase = LandingPhase::Success;
			speed = 0;
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
		if (ConnectionSerie::getValue("JB") != 0)
		{
			input = JOY_BAS;
		}
	}
	return input;
}
void Landing::saveScore()
{
	QFile file("Database.csv");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Impossible d’ouvrir le fichier.";
		return;
	}

	QTextStream in(&file);
	QStringList lines;

	while (!in.atEnd()) {
		QString line = in.readLine();
		QStringList parts = line.split(",");
		if (gameref->stat->getScore() > gameref->stat->previousScore) {
			if (parts.size() == 2 && parts[0].trimmed() == stat->playerName) {
				// Remplace le score
				line = stat->playerName + "," + QString::number(stat->getScore());
				stat->previousScore = stat->getScore();
			}
		}
		lines.append(line);
	}
	file.close();
	// Réécriture du fichier avec les nouvelles lignes
	if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Truncate)) {
		qWarning() << "Erreur lors de l’écriture du fichier.";
		return;
	}

	QTextStream out(&file);
	for (const QString& line : lines) {
		out << line << "\n";
	}

	file.close();
}