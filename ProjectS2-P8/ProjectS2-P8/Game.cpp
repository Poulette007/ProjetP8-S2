#include "Game.h"
#include "Takeoff.h"	//a garder pour eviter inclusion circulaire
#include "Landing.h"	//a garder pour eviter inclusion circulaire
#include <GameOver.h>

#define ACTOR_POS_X 1920
#define START_PLANE_X int(1920/3)

Game::Game(Stat* s, QStackedWidget* stack, GameOver* gameOverPage)
{
	BackGroundVol = new QGraphicsPixmapItem();
	BackGroundVol->setPixmap(QPixmap(ImageManager::getInstance().getImage(BACKGROUND_ATTERRISSAGE_DECOLAGE)));
	gameScene->addItem(BackGroundVol);
	BackGroundVol->setPos(0, 0);
	this->gameOver = gameOverPage;

	listActor = vector<Actor*>();	
	plane = new Plane(START_PLANE_X, 1080/4);
	gameScene->addItem(plane);
	plane->setPos(WIDTH_DASHBOARD, 2 * 1080 / 4);
	plane->show();
	this->stack = stack;
	stat = s;
	isCollision = false;
	count = 0;
	landingCount = 200;
	msg_envoi["led"] = 1;
	promptText = new FormatTextPixmap("", nullptr, 13, TEXTE, Qt::yellow);
	promptText->setPos(500, 100);
	gameScene->addItem(promptText);
	takeoff = new Takeoff(this, plane, stat, promptText, stack, gameOverPage);
	//state = Gamestate::Decollage;
	
}
void Game::readKeyBoardGame()
{

	if (GetAsyncKeyState('W') < 0)
	{
		stat->readKeybord('W');
	}
	if (GetAsyncKeyState('S') < 0)
	{
		stat->readKeybord('S');
	}
	if (GetAsyncKeyState('A') < 0)
	{
		stat->readKeybord('A');
	}
	if (GetAsyncKeyState('D') < 0)
	{
		stat->readKeybord('D');
	}
	if (ConnectionSerie::hasData())
	{
		if (ConnectionSerie::getValue("JH") == 1)
		{
			stat->readKeybord('W');
		}
		if (ConnectionSerie::getValue("JB") == 1)
		{
			stat->readKeybord('S');
		}
		int pot = ConnectionSerie::getValue("pot");
		stat->readManette(pot);
	}
}
void Game::update()
{
	
 	switch (state) {
	case Gamestate::Decollage:
		//updateGameplay();
		takeoff->updateTakeoff();
		break;
	case Gamestate::Gameplay:
		updateGameplay();
		break;
	case Gamestate::Landing:
		landing->updateLanding();
		break;
	case Gamestate::GameOver:
		break;
	}
}
void Game::updateGameplay()
{
	if(stat->close){
		etteinManette();
		state = Gamestate::GameOver;
		gameOver->setVictoire(false);
		stack->setCurrentWidget(gameOver);
		return;
	}
	for (auto actor : listActor)
	{
		actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());

		if (actor->x() <= START_PLANE_X - actor->pixmap().width()) {
			gameScene->removeItem(actor); // Retirer l'acteur de la scène Qt
			listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
			delete actor;
		}
	}
	stat->changeScore(1);
	stat->countFuel();
	plane->setPos(plane->x(), stat->getHeight());
	afficherStat();
	manageCollision();
	if (possibleLanding())
	{
		promptText->setPlainText("Atterrissage possible, allez a la plus haute altitude possible,\n augmentez votre vitesse en haut de 15000 kmh, et appuyez sur k (ou cercle) pout initialiser la sequence datterissage!");
		qDebug() << "speed:" << stat->getSpeed();
		if (((GetAsyncKeyState('K') < 0 || ConnectionSerie::getValue("BD")==0)) && plane->y()<1080 / 4 && stat->getSpeed()>10)
		{
			for (auto actor : listActor)
			{
				// actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());
				gameScene->removeItem(actor); // Retirer l'acteur de la scène Qt
				listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
				delete actor;
				
			}
			landing = new Landing(this, plane, stat, promptText, stack, gameOver);
			state = Gamestate::Landing;
		}
	}
	else
	{
		promptText->setPlainText("");
	}
}
bool Game::possibleLanding()
{
	qDebug() << "count:" << count;
	qDebug() << "landingCount:" << landingCount;
	if (plane->y() < 360 && count >= landingCount && count <= (landingCount + 150))
	{
		return true;
	}
	else if (count == landingCount + 150)
	{
		count = 0;
		landingCount += 150;
	}
	count++;
	return false;
}
void Game::changePlanePixmap()
{
	switch (stat->skinPlane)
	{
	case 0: // Plane
		plane->changePixmap(PLANE);
		break;
	case 1: // Chopper
		plane->changePixmap(CHOPPER);
		break;
	case 2: // Jet
		plane->changePixmap(JET);
		break;
	default:
		plane->changePixmap(PLANE);
		break;
	}
}
void Game::manageCollision()
{
	for (int i = listActor.size() - 1; i >= 0; --i)
	{
		if (listActor[i]->x() <= plane->x() + plane->pixmap().width() && listActor[i]->y() == plane->y())
		{
			CollionDetected(listActor[i]);
			gameScene->removeItem(listActor[i]);
			delete listActor[i];
			listActor.erase(listActor.begin() + i);
		}
	}
}
void Game::CollionDetected(Actor* actor)
{
	actor->action(*stat);
}
void Game::gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (coord));
}
void Game::afficherStat()
{
	
	afficherStatManette();
	afficherStatOnGame();
	
}
void Game::afficherStatManette()
{
	//affiche gaz sur bargraph manette
	if (stat->getFuel() > 100)
	{
		msg_envoi["BAR"] = 10;
	}
	else if (stat->getFuel() > 90 && stat->getFuel() < 100)
	{
		msg_envoi["BAR"] = 9;
	}
	else if (stat->getFuel() > 80 && stat->getFuel() < 90)
	{
		msg_envoi["BAR"] = 8;
	}
	else if (stat->getFuel() > 70 && stat->getFuel() < 80)
	{
		msg_envoi["BAR"] = 7;
	}
	else if (stat->getFuel() > 60 && stat->getFuel() < 70)
	{
		msg_envoi["BAR"] = 6;
	}
	else if (stat->getFuel() > 50 && stat->getFuel() < 60)
	{
		msg_envoi["BAR"] = 5;
	}
	else if (stat->getFuel() > 40 && stat->getFuel() < 50)
	{
		msg_envoi["BAR"] = 4;
	}
	else if (stat->getFuel() > 30 && stat->getFuel() < 40)
	{
		msg_envoi["BAR"] = 3;
	}
	else if (stat->getFuel() > 20 && stat->getFuel() < 30)
	{
		msg_envoi["BAR"] = 2;
	}
	else if (stat->getFuel() > 10 && stat->getFuel() < 20)
	{
		msg_envoi["BAR"] = 1;
	}
	else if (stat->getFuel() < 10)
	{
		msg_envoi["BAR"] = 0;
	}
	msg_envoi["LCD"] = "score: " + std::to_string(stat->getScore());
	ConnectionSerie::Envoie(msg_envoi);
}
void Game::etteinManette()
{
	msg_envoi["BAR"] = 0;
	msg_envoi["led"] = 0;
	msg_envoi["LCD"] = "score: " + std::to_string(stat->getScore())+ "Partie termine!";
	ConnectionSerie::Envoie(msg_envoi);
}
void Game::afficherStatOnGame()
{
	dash->update();
}
void Game::setupStatOnGame()
{
	dash = new QDashboard(stat);
	gameScene->addItem(dash);
	
}     
void Game::generateObstacles() {
	if (state == Gamestate::Gameplay) {

		random = rand() % 7;
		obstacle = rand() % 3;
		posY = rand() % 3;
		distance = 0;
		switch (random)
		{
		case 0: // Deux obstacles côte à côte
			distance = rand() % 400 + 150;
			createObstacle(obstacle, posY);
			createObstacle(rand() % 4, (posY + 1) % 3);
			break;
		case 1: // Muon et obstacle
			distance = rand() % 400 + 200;
			createObstacle(obstacle, posY);
			stat->muonTrue = true;
			break;
		case 2: // Obstacle simple
			createObstacle(obstacle, posY);
			break;
		case 3: // Mur complet
			for (int i = 0; i < 3; ++i) {
				createObstacle(rand() % 4, i);
				distance = rand() % 400 + 200 * i;
			}
			break;
		case 4: // Centre seulement
			// distance = rand() % 100 + 50;
			createObstacle(rand() % 4, 1);
			break;
		case 5: // Trou au milieu
			//distance = rand() % 100 + 75;
			createObstacle(rand() % 4, 0);
			distance = rand() % 400 + 150;
			createObstacle(rand() % 4, 2);
			stat->muonTrue = true;
			break;
		case 6: // 2 vent + 1 random
			createObstacle(1, posY);
			distance = 350;
			createObstacle(1, posY);
			distance = random % 400 + 180;
			createObstacle(rand() % 4, (posY + 1) % 3);
			break;
		}
		if (stat->muonTrue)
		{
			listActor.push_back(new Tree(ACTOR_POS_X, 1080 * 3 / 4));
			gameScene->addItem(listActor.back());
			stat->muonTrue = false;
		}
	}

}
void Game::createObstacle(int obstacle, int posY)
{
	int yPos = posY * RANGEE_HEIGHT;
	if (obstacle == 0) {
		listActor.push_back(new Wind(ACTOR_POS_X + distance, yPos));
		gameScene->addItem(listActor.back());
	}
	else if (obstacle == 1) {
		listActor.push_back(new Gaz(ACTOR_POS_X + distance, yPos));
		gameScene->addItem(listActor.back());
	}
	else if (obstacle == 2) {
		listActor.push_back(new Bird(ACTOR_POS_X + distance, yPos));
		gameScene->addItem(listActor.back());
	}
}