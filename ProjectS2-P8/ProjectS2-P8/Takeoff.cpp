#include "Takeoff.h"
#include "ImageManager.h"
#include "const.h"
Takeoff::Takeoff(Game* game, Plane* p, Stat* s, QGraphicsTextItem* prompt)
{
	initPiste();
	takeoffTimer = new QTimer(this);
	connect(takeoffTimer, &QTimer::timeout, this, &Takeoff::updateTakeoff);
	takeoffTimer->start(30);
	plane = p;
	stat = s;
	gameref = game;
	promptText = prompt;
	inputCount = 0;
	plane->setPos(0, 1080-plane->pixmap().height()-10);
	plane->setZValue(1);	//1er plan
	shuffleDirection();
}
void Takeoff::initPiste()
{
	longeurPiste = QRandomGenerator::global()->bounded(100, 200);
	for (int i = 0; i < longeurPiste; i++)
	{
		QGraphicsPixmapItem* runwayTile = new QGraphicsPixmapItem();
		runwayTile->setPixmap(ImageManager::getInstance().getImage(RUNWAY));
		runwayTile->setScale(0.5);
		runwayTile->setPos(i*runwayTile->pixmap().width()/2, 1080-runwayTile->pixmap().height()/2);
		runwayTile->setZValue(0);	//arriere plan
		gameScene->addItem(runwayTile);
		runwayTilePixmap.push_back(runwayTile);
	}
}
void Takeoff::updateTakeoff()
{
	//qDebug() << "Takeoff phase:" << static_cast<int>(takeoffPhase);

	//pour le defilement de la piste
	if (takeoffPhase != TakeoffPhase::AccelerationInitiale)
	{
		int scrollSpeed = std::max(5, speed / 5);
		for (auto tile : runwayTilePixmap) {
			tile->setPos(tile->x() - scrollSpeed, tile->y());
		}
	}
	if (takeoffPhase == TakeoffPhase::Acceleration || takeoffPhase == TakeoffPhase::Height) 
	{
		QGraphicsPixmapItem* lastTile = runwayTilePixmap.back();
		if (lastTile->x() + lastTile->pixmap().width() * lastTile->scale() < plane->x()) {
			takeoffPhase = TakeoffPhase::Failure;
			qDebug() << "you crashed.";
		}
	}
	switch (takeoffPhase) {
	case TakeoffPhase::AccelerationInitiale:
		updateAccelerationInitiale();
		break;
	case TakeoffPhase::Acceleration:
		updateAcceleration();
		break;
	case TakeoffPhase::Height:
		updateHeight();
		break;
	case TakeoffPhase::Success:
		animationTakeoff();
		break;
	case TakeoffPhase::Failure:
		break;
	}
}

void Takeoff::updateAccelerationInitiale()
{
	if (input == SPACEBAR || input == BOUTON_BAS)
	{
		speed += 10;
		inputCount++;
		if (inputCount < 8)
		{
			//petit deplacement initial
			plane->setPos(plane->x() + 50, plane->y());
		}
		else
		{
			takeoffPhase = TakeoffPhase::Acceleration;
		}
		input = 0;	//faut juste etre sur de le reset a cause de ma logique de merde
	}
}
void Takeoff::updateAcceleration()
{
	if (input == SPACEBAR || input == BOUTON_BAS)
	{
  		speed += 15;	
		input = 0;
	}
	countRalentissement++;
	if (countRalentissement % 2 == 0)
	{
		speed -= 3;
	}
	stat->changeSpeed(speed);
	if (speed >= recquiredSpeed)
	{
		takeoffPhase = TakeoffPhase::Height;
	}
	QString vitesse = QString::number(speed);
	QString vitesseRecquise = QString::number(recquiredSpeed);
	//qDebug() << "Vitesse:" << vitesse << "Vitesse recquise:" << vitesseRecquise;
	promptText->setPlainText("Vitesse: " + vitesse + "Vitesse recquise: "+ vitesseRecquise);
	//qDebug() << "Scrolling runway at speed" << scrollSpeed;
}
void Takeoff::updateHeight()
{
	//affichage de la direction recquise a lecran
	QString prompt = QString::fromStdString("Appuyez sur " + directionPrompt);
	promptText->setPlainText("Appuyez sur: " + QString::fromStdString(directionPrompt));
 	if (directionPrompt == "haut")
	{
		if (input == CLAVIER_W || input == BOUTON_HAUT)
		{ 
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if(input == CLAVIER_A || input == CLAVIER_S || input == CLAVIER_D)
		{
			failCount++;
			input = 0;
		}
		
		
	}
	else if (directionPrompt == "gauche")
	{
		if (input == CLAVIER_A || input == BOUTON_GAUCHE) 
		{
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if (input == CLAVIER_W || input == CLAVIER_S || input == CLAVIER_D)
		{
			failCount++;
			input = 0;
		}
	}
	else if (directionPrompt == "droite")
	{
		if (input == CLAVIER_D || input == BOUTON_DROIT)
		{
			successCount++;
			input = 0;
			shuffleDirection();
		}
		else if (input == CLAVIER_W || input == CLAVIER_S || input == CLAVIER_A)
		{
			failCount++;
			input = 0;
		}
	}
	else if (directionPrompt == "bas")
	{
 		if (input == CLAVIER_S || input == BOUTON_BAS) 
		{
			successCount++;
			shuffleDirection();
			input = 0;
		}
		else if (input == CLAVIER_W || input == CLAVIER_A || input == CLAVIER_D)
		{
			failCount++;
			input = 0;
		}	
	}
	
	if (successCount >= 6) {
		takeoffPhase = TakeoffPhase::Success;
	}
	else if (failCount >= 10) {
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
	}
	return input;
}

void Takeoff::shuffleDirection()
{
	static vector<string> directions = { "haut", "gauche", "droite", "bas" };
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
		if (plane->y() > 1080 / 3)
		{
			plane->setPos(plane -> x(), plane->y() - 5);
		}
		else 
		{
			plane->setRotation(0);
		}
		qDebug() << "toute la piste:" << touteLaPiste;
		if (touteLaPiste && plane->y() < 1080 / 2)
		{
			plane->setRotation(0);
			takeoffTimer->stop();
			promptText->setPlainText("");
			gameref->state = Game::Gamestate::Gameplay;
		}
}