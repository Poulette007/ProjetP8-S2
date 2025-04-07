#include "Game.h"
#include "Takeoff.h"	//a garder pour eviter inclusion circulaire
#include "Landing.h"	//a garder pour eviter inclusion circulaire

#define ACTOR_POS_X 1920
#define START_PLANE_X int(1920/3)


Game::Game(Stat* s, QStackedWidget* stack, GameOver* gameOverPage)
{
	//BackGroundVol = new QGraphicsPixmapItem();
	//BackGroundVol->setPixmap(QPixmap("sprites/background/BackgroundVol").scaled(1920 + 10, 1080 + 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	gameScene->addItem(BackGroundVol);
	//BackGroundVol->show();
	//BackGroundVol->setPos(1920 / 3 - 10, -10);

	listActor = vector<Actor*>();
	plane = new Plane(START_PLANE_X, 1080/4);
	gameScene->addItem(plane);
	plane->setPos(0, 2 * 1080 / 4);
	plane->show();
	
	stat = s;
	isCollision = false;
	count = 0;
	landingCount = 200;
	msg_envoi["led"] = 1;
	promptText = new QGraphicsTextItem();
	promptText->setDefaultTextColor(Qt::yellow);
	promptText->setFont(QFont("Consolas", 24));
	promptText->setPos(100, 100);
	gameScene->addItem(promptText);

	//takeoff = new Takeoff(this, plane, stat, promptText, stack, gameOverPage);
	state = Gamestate::Gameplay;
	
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
}
void Game::update()
{
	switch (state) {
	case Gamestate::Decollage:
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
	for (auto actor : listActor)
	{
		actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());

		if (actor->x() <= START_PLANE_X - actor->pixmap().width()) {
			gameScene->removeItem(actor); // Retirer l'acteur de la scène Qt
			listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
			delete actor;
		}
	}
	stat->changeScore(250);
	stat->countFuel();
	plane->setPos(plane->x(), stat->getHeight());
	afficherStat();
	manageCollision();
	generateObstacles();
	if (/*possibleLanding()*/true)
	{
		promptText->setPlainText("Atterrissage possible, allez a la plus haute altitude possible et appuyez sur k pout initialiser la sequence datterissage!");
		if (GetAsyncKeyState('K') < 0 && plane->y()<1080/4)
		{
			for (auto actor : listActor)
			{
				actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());
				gameScene->removeItem(actor); // Retirer l'acteur de la scène Qt
				listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
				delete actor;
				
			}
			landing = new Landing(this, plane, stat, promptText);
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

void Game::manageCollision()
{
	for (int i = listActor.size() - 1; i >= 0; --i)
	{
		if (listActor[i]->x() <= plane->x() + plane->pixmap().width() && listActor[i]->y() == plane->y())
		{
			CollionDetected(listActor[i]);

			// Supprimer l'élément de la scène
			gameScene->removeItem(listActor[i]);

			// Libérer la mémoire
			delete listActor[i];

			// Supprimer l'élément du tableau
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
	// DEBUG
	gotoxy(0, 6);
	cout << "Gaz: " << stat->getFuel() << endl;
	cout << "Vitesse: " << stat->getSpeed() << endl;
	cout << "Pointage: " << stat->getScore() << endl;
	cout << "Hauteur: " << stat->getHeight() << endl;
	
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

void Game::afficherStatOnGame()
{
	dash->update();
}

void Game::setupStatOnGame()
{
	dash = new QDashboard(stat);
	gameScene->addItem(dash);
	
}



#define TREE_SPAWN_MIN 45        // Minimum pour spawn un arbre
#define TREE_SPAWN_MAX 50        // Maximum pour spawn un arbre
#define WIND_SPAWN_MIN 1
#define WIND_SPAWN_MAX 4
#define GAS_SPAWN_MIN 10
#define GAS_SPAWN_MAX 13   
#define BIRD_SPAWN_MIN 20       
#define BIRD_SPAWN_MAX 25       
#define TREE_MIN_DIST 500       
#define OBSTACLE_MIN_DIST 700   
#define SCREEN_HEIGHT 1080      
#define RANGEE_HEIGHT (SCREEN_HEIGHT / 4) 

void Game::generateObstacles()
{
	int random = rand() % 2000;
	int posY = rand() % 3;
	bool tree = false;

	if (random >= TREE_SPAWN_MIN && random <= TREE_SPAWN_MAX)
	{
		for (auto actor : listActor)
		{
			if (actor->y() == 3) 
			{

				if (abs(actor->x() - ACTOR_POS_X) < TREE_MIN_DIST)
				{
					tree = true;
					break;
				}
			}
		}
		if (!tree) {
			listActor.push_back(new Tree(ACTOR_POS_X, 3 * RANGEE_HEIGHT)); // Spawne un arbre dans la dernière rangée
			gameScene->addItem(listActor.back());
		}
	}

	if (isPosYPossible(posY))
	{
		int yPos = posY * RANGEE_HEIGHT;

		if (random >= WIND_SPAWN_MIN && random <= WIND_SPAWN_MAX) {
			listActor.push_back(new Wind(ACTOR_POS_X, yPos));
			gameScene->addItem(listActor.back());
		}
		else if (random >= GAS_SPAWN_MIN && random <= GAS_SPAWN_MAX) {
			listActor.push_back(new Gaz(ACTOR_POS_X, yPos));
			gameScene->addItem(listActor.back());
		}
		else if (random >= BIRD_SPAWN_MIN && random <= BIRD_SPAWN_MAX) {
			listActor.push_back(new Bird(ACTOR_POS_X, yPos));
			gameScene->addItem(listActor.back());
		}
	}
}

bool Game::isPosYPossible(int y)
{
	for (auto actor : listActor)
	{
		if (actor->y() == y)
		{
			if (abs(actor->x() - ACTOR_POS_X) < OBSTACLE_MIN_DIST) 
				return false; 
		}
	}
	return true;
}
