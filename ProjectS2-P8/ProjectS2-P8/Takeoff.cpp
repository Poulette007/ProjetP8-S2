#include "Takeoff.h"
#include "ImageManager.h"
#include "const.h"
Takeoff::Takeoff(Game* game, Plane* p, Stat* s, FormatTextPixmap* prompt, QStackedWidget* stack, GameOver* gameOverPage)
{
	
	takeoffTimer = new QTimer(this);
	connect(takeoffTimer, &QTimer::timeout, this, &Takeoff::updateTakeoff);
	takeoffTimer->start(30);
	plane = p;
	stat = s;
	gameref = game;
	promptText = prompt;
	this->stack = stack;
	this->gameOver = gameOverPage;
	inputCount = 0;
	initPiste();
	plane->setPos(0, 1080-plane->pixmap().height()-10);
	plane->setZValue(2);	//1er plan
	shuffleDirection();
}
void Takeoff::initPiste()
{
	longeurPiste = QRandomGenerator::global()->bounded(100, 250);
	// Chargement et redimensionnement de l'aéroport
	QPixmap originalAirport = ImageManager::getInstance().getImage(AIRPORT);
	QPixmap resizedAirport = originalAirport.scaled(800, 700, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	QGraphicsPixmapItem* airport = new QGraphicsPixmapItem(resizedAirport);

	// Positionnement
	airport->setPos(500, 1080 - resizedAirport.height() - 50);
	// Z-order et ajout à la scène
	airport->setZValue(0);
	//controlTower->setZValue(0);
	gameScene->addItem(airport);
	//gameScene->addItem(controlTower);
	// Ajout au vecteur de défilement
	runwayTilePixmap.push_back(airport);
	//runwayTilePixmap.push_back(controlTower);

	int i = 0;
	for (i = 0; i < longeurPiste; i++)
	{
		QGraphicsPixmapItem* runwayTile = new QGraphicsPixmapItem();
		runwayTile->setPixmap(ImageManager::getInstance().getImage(RUNWAY));
		runwayTile->setScale(0.65);
		runwayTile->setPos(i * runwayTile->pixmap().width() / 2 - 40, 1080 - (runwayTile->pixmap().height() / 2) - 60);
		runwayTile->setZValue(1);
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
	promptText->setPlainText("Il faut rechauffer le moteur avant le decollage, montez le levier de vitesse: ");
}
void Takeoff::updateTakeoff()
{
	gameref->afficherStatManette();
	//pour le defilement de la piste
	if (takeoffPhase != TakeoffPhase::AccelerationInitiale && takeoffPhase != TakeoffPhase::Rechauffement)
	{
		int scrollSpeed = std::max(5, speed / 5);
		for (auto tile : runwayTilePixmap) {
			tile->setPos(tile->x() - scrollSpeed, tile->y());
		}
		gameref->afficherStat();
	}
	if (takeoffPhase == TakeoffPhase::Acceleration || takeoffPhase == TakeoffPhase::Height) 
	{
		QGraphicsPixmapItem* lastTile = runwayTilePixmap.back();
		if (lastTile->x() + lastTile->pixmap().width() * lastTile->scale() < plane->x()) {
			takeoffPhase = TakeoffPhase::Failure;
		}
	}
	switch (takeoffPhase) {
	case TakeoffPhase::Rechauffement:
		updateRechauffement();
		break;
	case TakeoffPhase::AccelerationInitiale:
		updateAccelerationInitiale();
		break;
	case TakeoffPhase::Acceleration:
		updateAcceleration();
		break;
	case TakeoffPhase::ChangerPot:
		updateChangerPot();
		break;
	case TakeoffPhase::Height:
		updateHeight();
		break;
	case TakeoffPhase::Success:
		animationTakeoff();
		break;
	case TakeoffPhase::Failure:
		gameref->etteinManette();
		gameOver->setVictoire(false); 
		stack->setCurrentWidget(gameOver);
		gameref->state = Game::Gamestate::GameOver;
		break;
	}
}
void Takeoff::updateRechauffement()
{
	int pot =abs((ConnectionSerie::getValue("pot")));
	if (pot >700)
		pot = 700; // Limite la valeur à 700
	pot = (pot / 7) % 100; // Convertit la valeur en pourcentage
	if (pot == 0 && !moteurChaud)
	{
		pot = lastPot;
		return;
	}
		
	lastPot = pot;
	QString intensiterMot = QString::number(pot);
	qDebug ()<< "pot: " << pot;
	if (moteurChaud)
	{
		promptText->setPlainText("Le moteur est chaud, decollage imminent. Redescendre le levier a 0: "+ intensiterMot+" /100");
		if (pot == 0)
		{
			pot = lastPot;
			return;
		}
		if (pot < 20)
			takeoffPhase = TakeoffPhase::AccelerationInitiale;
	}
	else if (pot < 90)
		promptText->setPlainText("Il faut rechauffer le moteur avant le decollage, montez le levier de vitesse: " + intensiterMot + " /100");
	
	else if (pot > 93)
	{
		moteurChaud = true;
	}
	
}
void Takeoff::updateChangerPot()
{
	int pot = abs((ConnectionSerie::getValue("pot")));
	pot = (pot / 7) % 100; // Convertit la valeur en pourcentage
	promptText->setPlainText("Ajustez le levier de vitesse a la moitie de la course: " + QString::number(pot) + " /100");
	if (pot > 40 && pot < 60)
	{
		stat->readManette(pot);		//fix le speed du jeu a la valeur du pot
		takeoffPhase = TakeoffPhase::Height;
	}
}
void Takeoff::updateAccelerationInitiale()
{
	QString vitesse = QString::number(speed);
	QString vitesseRecquise = QString::number(recquiredSpeed);
	promptText->setPlainText("Agitez la manette pour accelerer! Vitesse: " + vitesse + "Vitesse recquise: " + vitesseRecquise);
	if (input == SPACEBAR || input == BOUTON_BAS || input == SHAKE)
	{
		speed += 10;
		inputCount++;
		if (inputCount < 8)
		{
			//petit deplacement initial
			stat->changeSpeed(1);
			plane->setPos(plane->x() + 50, plane->y());
		}
		else
		{
			if (tower) {
				gameScene->removeItem(tower); // retire de la scène
				delete tower;                 // libère la mémoire
				tower = nullptr;
			}

			if (airport) {
				gameScene->removeItem(airport);
				delete airport;
				airport = nullptr;
			}
			plane->setPos(1080/5 + (plane->pixmap().width() / 2) + 40 , plane->y());
			gameref->setupStatOnGame();
			takeoffPhase = TakeoffPhase::Acceleration;
		}
		input = 0;	//faut juste etre sur de le reset a cause de ma logique de merde
	}
}
void Takeoff::updateAcceleration()
{
	if (input == SPACEBAR || input == BOUTON_BAS || input == SHAKE)
	{
  		speed += 8;	
		//stat->setSpeed(speed % 10 + 2);
		input = 0;
	}
	countRalentissement++;
	if (countRalentissement % 2 == 0)
	{
		//stat->setSpeed(speed % 10 + 2);
		speed -= 1;
		if (speed < 10)
			speed = 10;
	}
	// stat->changeSpeed(speed);
	if (speed >= recquiredSpeed)
	{
		takeoffPhase = TakeoffPhase::ChangerPot;
	}
	QString vitesse = QString::number(speed);
	QString vitesseRecquise = QString::number(recquiredSpeed);
	promptText->setPlainText("Agitez la manette pour accelerer! Vitesse: " + vitesse + "  Vitesse recquise: "+ vitesseRecquise);
}
void Takeoff::updateHeight()
{
	//affichage de la direction recquise a lecran
	QString prompt = QString::fromStdString("Appuyez sur " + directionPrompt);
	promptText->setPlainText("Appuyez sur: " + QString::fromStdString(directionPrompt)+ " Reussite: "+ QString::number(successCount)+ 
		"/4 Echec: "+ QString::number(failCount)+ "/5");

 	if (directionPrompt == "haut")
	{
		if (input == CLAVIER_W || input == BOUTON_HAUT)
		{ 
			stat->changeFuel(10);
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if(input == CLAVIER_A || input == CLAVIER_S || input == CLAVIER_D)
		{
			stat->changeFuel(-5);
			failCount++;
			input = 0;
		}
		
		
	}
	else if (directionPrompt == "gauche")
	{
		if (input == CLAVIER_A || input == BOUTON_GAUCHE) 
		{
			stat->changeFuel(10);
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if (input == CLAVIER_W || input == CLAVIER_S || input == CLAVIER_D || input == BOUTON_DROIT || input == BOUTON_HAUT || input == BOUTON_BAS)
		{
			stat->changeFuel(-5);
			failCount++;
			input = 0;
		}
	}
	else if (directionPrompt == "droite")
	{
		if (input == CLAVIER_D || input == BOUTON_DROIT)
		{
			stat->changeFuel(10);
			successCount++;
			input = 0;
			shuffleDirection();
		}
		else if (input == CLAVIER_W || input == CLAVIER_S || input == CLAVIER_A || input == BOUTON_HAUT || input == BOUTON_BAS|| input == BOUTON_GAUCHE)
		{
			stat->changeFuel(-5);
			failCount++;
			input = 0;
		}
	}
	else if (directionPrompt == "bas")
	{
 		if (input == CLAVIER_S || input == BOUTON_BAS) 
		{
			stat->changeFuel(10);
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if (input == CLAVIER_W || input == CLAVIER_A || input == CLAVIER_D || input == BOUTON_HAUT || input == BOUTON_DROIT || input == BOUTON_GAUCHE)
		{
			stat->changeFuel(-5);
			failCount++;
			input = 0;
		}	
	}
	
	if (successCount >= 4) {
		promptText->setPlainText("Decollage reussi!");
		//stat->setSpeed(15);
		takeoffPhase = TakeoffPhase::Success;
	}
	else if (failCount >= 5) {
		takeoffPhase = TakeoffPhase::Failure;
	}

}
int Takeoff::readInputDecollage()
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
		if (ConnectionSerie::getValue("AX") == 1)
		{
			input = SHAKE;
		}
	}
	return input;
}

void Takeoff::shuffleDirection()
{
	static vector<string> directions = { "haut", "gauche", "droite", "bas" };
	//vector<string> directions = { "Triangle", "Carre", "Cercle", "X" };
	std::shuffle(directions.begin(), directions.end(), std::random_device());
	directionPrompt = directions.front();
}

void Takeoff::animationTakeoff()
{
	int scrollSpeed = std::max(5, speed / 5);
	bool touteLaPiste = true;
	plane->setRotation(-45);
	for (auto tile : runwayTilePixmap)
	{
		tile->setPos(tile->x() - scrollSpeed, tile->y());
		if (tile->x() + tile->pixmap().width() * 0.5 > 0)
		{
			touteLaPiste = false;
		}
		else 
		{ 
			touteLaPiste = true;
		}
	}
		if (plane->y() > 1080 / 4)
		{
			plane->setPos(plane -> x(), plane->y() - 5);
		}
		else 
		{
			if (upPlane) {
				stat->changeHeight(-1080 / 4);
				stat->changeHeight(-1080 / 4);
				upPlane = false;
			}
			plane->setRotation(0);
		}
		if (touteLaPiste && plane->y() < 1080 / 2)
		{
			plane->setRotation(0);
			takeoffTimer->stop();
			promptText->setPlainText("");
			gameref->state = Game::Gamestate::Gameplay;
			BackGroundVol->setPixmap(ImageManager::getInstance().getImage(BACKGROUND_GAME));
		}
}