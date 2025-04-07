#include "Game.h"
#include "Takeoff.h"	//a garder pour eviter inclusion circulaire
#include "Landing.h"	//a garder pour eviter inclusion circulaire
#include <GameOver.h>

#define ACTOR_POS_X 1920
#define START_PLANE_X int(1920/3)


Game::Game(Stat* s, QStackedWidget* stack, GameOver* gameOverPage)
{
	BackGroundVol = new QGraphicsPixmapItem();
	BackGroundVol->setPixmap(QPixmap("sprites/background/BackgroundVol").scaled(1920 + 10, 1080 + 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
	gameScene->addItem(BackGroundVol);
	BackGroundVol->show();
	BackGroundVol->setPos(WIDTH_DASHBOARD, -10);

	listActor = vector<Actor*>();
	plane = new Plane(START_PLANE_X, 1080/4);
	gameScene->addItem(plane);
	plane->setPos(WIDTH_DASHBOARD, 2 * 1080 / 4);
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
	//takeoff = new Takeoff(this, plane, stat, promptText);


	// Dashboard 
	setupStatOnGame();
	state = Gamestate::Gameplay;

	

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
}
void Game::update()
{
	
	switch (state) {
	//case Gamestate::Decollage:
	//	//updateGameplay();
	//	takeoff->updateTakeoff();
	//	break;
	case Gamestate::Gameplay:
		updateGameplay();
		break;
	case Gamestate::Landing:
		landing->updateLanding();
		break;
	case Gamestate::GameOver:
		overGame = new GameOver();
		break;
	}
}
void Game::updateGameplay()
{
	if(stat->close){
		Gamestate::GameOver;
 		qDebug() << "Game Over";
		return;
	}
	for (auto actor : listActor)
	{
		actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());

		if (actor->x() <= START_PLANE_X - actor->pixmap().width()) {
			gameScene->removeItem(actor); // Retirer l'acteur de la sc�ne Qt
			listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
			delete actor;
		}
	}
	stat->changeScore(1);
	stat->countFuel();
	plane->setPos(plane->x(), stat->getHeight());
	afficherStat();
	manageCollision();
	if (false)
	{
		promptText->setPlainText("Atterrissage possible, allez a la plus haute altitude possible et appuyez sur k pout initialiser la sequence datterissage!");
		if (GetAsyncKeyState('K') < 0 && plane->y()<1080/4)
		{
			for (auto actor : listActor)
			{
				actor->setPos(actor->x() - (actor->getSpeed() * stat->getSpeed()), actor->y());
				gameScene->removeItem(actor); // Retirer l'acteur de la sc�ne Qt
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


void Game::generateObstacles() {
	random = rand() % 7;
	obstacle = rand() % 3;
	posY = rand() % 3;
	distance = 0;
	switch (random)
	{
	case 0: // Deux obstacles c�te � c�te
		distance = rand() % 400;
		createObstacle(obstacle, posY);
		createObstacle(rand() % 4, (posY + 1) % 3);
		break;
	case 1: // Muon et obstacle
		distance = rand() % 400;
		createObstacle(obstacle, posY);
		stat->muonTrue = true;
		break;
	case 2: // Obstacle simple
		createObstacle(obstacle, posY);
		break;
	case 3: // Mur complet
		for (int i = 0; i < 3; ++i){
			createObstacle(rand() % 4, i);
			distance = rand() % 150 + 50 * i;
		}
		break;
	case 4: // Centre seulement
		distance = rand() % 100 + 50;
		createObstacle(rand() % 4, 1);
		break;
	case 5: // Trou au milieu
		distance = rand() % 100 + 75;
		createObstacle(rand() % 4, 0);
		distance = rand() % 200 + 150;
		createObstacle(rand() % 4, 2);
		stat->muonTrue = true;
		break;
	case 6: // 2 vent + 1 random
		createObstacle(1, posY);
		distance = 350;
		createObstacle(1, posY);
		distance = random % 400;
		createObstacle(rand() % 4, (posY + 1 )% 3);
		break;
	}

	if (stat->muonTrue)
	{
		listActor.push_back(new Tree(ACTOR_POS_X, 1080 * 3/4));
		gameScene->addItem(listActor.back());
		stat->muonTrue = false;
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
//void Game::generateObstacles()
//{
//
//	int random = rand() % 2000;
//	int posY = rand() % 3;
//	bool tree = false;
//
//	if (random >= TREE_SPAWN_MIN && random <= TREE_SPAWN_MAX)
//	{
//		for (auto actor : listActor)
//		{
//			if (actor->y() == 3) 
//			{
//
//				if (abs(actor->x() - ACTOR_POS_X) < TREE_MIN_DIST)
//				{
//					tree = true;
//					break;
//				}
//			}
//		}
//		if (!tree) {
//			listActor.push_back(new Tree(ACTOR_POS_X, 3 * RANGEE_HEIGHT)); // Spawne un arbre dans la derni�re rang�e
//			gameScene->addItem(listActor.back());
//		}
//	}
//
//	if (isPosYPossible(posY))
//	{
//		int yPos = posY * RANGEE_HEIGHT;
//
//		if (random >= WIND_SPAWN_MIN && random <= WIND_SPAWN_MAX) {
//			listActor.push_back(new Wind(ACTOR_POS_X, yPos));
//			gameScene->addItem(listActor.back());
//		}
//		else if (random >= GAS_SPAWN_MIN && random <= GAS_SPAWN_MAX) {
//			listActor.push_back(new Gaz(ACTOR_POS_X, yPos));
//			gameScene->addItem(listActor.back());
//		}
//		else if (random >= BIRD_SPAWN_MIN && random <= BIRD_SPAWN_MAX) {
//			listActor.push_back(new Bird(ACTOR_POS_X, yPos));
//			gameScene->addItem(listActor.back());
//		}
//	}
//}
//
//bool Game::isPosYPossible(int y)
//{
//	for (auto actor : listActor)
//	{
//		if (actor->y() == y)
//		{
//			if (abs(actor->x() - ACTOR_POS_X) < OBSTACLE_MIN_DIST) 
//				return false; 
//		}
//	}
//	return true;
//}

/*

bool Game::touchDown()
{
	int state = 0;
	int ch = 0;
	int speed = 100;
	int count = 0;
	int startindex = 0;
	plane->setX(2);
	plane->setY(0);
	int length = rand() % 200 + 400;
	bool atterrissageGood = false;
	vector<char> piste(length + 200);

	bool joyBas = false;
	bool bpG = true;
	bool bpB = true;
	bool bpD = true;
	bool bpH = true;
	//On remplit la foret
	for (int i = 0; i < 200; i++)
	{
		piste[i] = 'T';
	}

	//On remplit la piste
	for (int i = 200; i < length; i++)
	{
		if (i % 10 == 1)
			piste[i] = '|';
		else if (i >= length - 5)
			piste[i] = 'X';
		else
			piste[i] = '-';
	}
	
	//Jeu d'aterrissage
	while (!atterrissageGood)
	{
		switch (state)
		{
			case 0:	//Appuyer plusieurs fois sur a pour diminuer la vitesse
				gotoxy(0, 8);
				cout << "Diminuez la vitesse de 50% (bouton gauche sur manette, 'A' sur clavier)";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpG = ConnectionSerie::getValue("BG");
				}
				else
					break;
				if (ch == 'a' || bpG == 0)
				{
					speed -= 5;
					ch = 0;
					bpG = 1;
				}
				else
					break;
				if (speed <= 50)
				{
					state = 1;
					gotoxy(0, 8);
					cout << "Diminuez la vitesse de 50%: OK!";
					bpG = 1;
				}
				else
					break;
				break;
			case 1: //Diminuer la hauteur de l'avion
				gotoxy(0, 9);
				cout << "Diminuez la hauteur de 2 positions";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					joyBas = ConnectionSerie::getValue("JB");
				}
				if (ch == 's' || joyBas==1)
				{
					gotoxy(plane->getX(), plane->getY());
					cout << "      "; //Supprimer l'avion pour la deplacer
					plane->setY(plane->getY() + 1);
					ch = 0;
					joyBas = 0;
				}
				else
					break;
				if (plane->getY() == 2)
				{
					gotoxy(0, 9);
					cout << "Diminuez la hauteur de 2 positions: OK!";
					state = 2;
				}
				else
					break;
				break;
			case 2:
				gotoxy(0, 10);
				cout << "Sortez le train d'atterissage en appuiant sur: T, ou BAS sur manette";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpB = ConnectionSerie::getValue("BB");
				}
				if (ch == 't' || bpB ==0)
				{
					gotoxy(0, 10);
					cout << "Sortez le train d'atterissage en appuiant sur: T: OK!";
					state = 3;
					bpB = 1;
				}
				else
					break;
				break;
			case 3:
				gotoxy(0, 11);
				cout << "Diminuez la vitesse de 25%";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpG = ConnectionSerie::getValue("BG");
				}
				else
					break;
				if (ch == 'a' || bpG==0)
				{
					speed -= 2;
					ch = 0;
					bpG = 1;
				}
				else
					break;
				if (speed <= 25)
				{
					state = 4;
					gotoxy(0, 11);
					cout << "Diminuez la vitesse de 25%: OK!";
					bpG = 1;
				}
				else
					break;
				break;
			case 4:
				gotoxy(0, 12);
				cout << "Touchez le sol";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					joyBas = ConnectionSerie::getValue("JB");
				}
				if (ch == 's' || joyBas==1)
				{
					gotoxy(plane->getX(), plane->getY());
					cout << "      ";
					plane->setY(plane->getY() + 1);
					ch = 0;
					joyBas = 0;
				}
				else
					break;
				if (plane->getY() == 5)
				{
					gotoxy(0, 12);
					cout << "Touchez le sol: OK!";
					state = 5;
					joyBas = 0;
				}
				else
					break;
				break;
			case 5:
				gotoxy(0, 13);
				cout << "Diminuez la vitesse a 10%";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpG = ConnectionSerie::getValue("BG");
				}
				else
					break;
				if (ch == 'a' || bpG==0)
				{
					speed -= 2;
					ch = 0;
					bpG = 1;
				}
				else
					break;
				if (speed <= 10)
				{
					state = 6;
					gotoxy(0, 13);
					cout << "Diminuez la vitesse a 10%: OK!";
					bpG = 1;
				}
				else
					break;
				break;
			case 6:
				gotoxy(0, 14);
				cout << "Activez la propultion invese en appuiant sur: P, ou haut pour la manette";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpH = ConnectionSerie::getValue("BH");
				}
				if (ch == 'p' || bpH==0)
				{
					gotoxy(0, 14);
					cout << "Activez la propultioninvese en appuiant sur: P: OK!";
					state = 7;
					bpH = 1;	
				}
				else
					break;
				break;
			case 7:
				count++;
				if (count >= 10)
				{
					speed -= 1;
					if (speed == 1)
					{
						state = 8;
					}
					else
					{
						count = 0;
						break;
					}
				}
				else
					break;
			case 8:
				gotoxy(0, 15);
				cout << "Immobiliser l'avion avec les freins en appuiant sur: F, ou droit sur manette";
				if (_kbhit())
					ch = _getch();
				else if (ConnectionSerie::hasData())
				{
					bpD = ConnectionSerie::getValue("BD");
				}
				if (ch == 'f' || bpD == 0)
				{
					gotoxy(0, 10);
					cout << "Immobiliser l'avion avec les freins en appuiant sur: F: OK!";
					gotoxy(0, 7);
					cout << "Vitesse: 0    ";
					gotoxy(35, 3);
					cout << "Atterrissage reussi!!!";
					atterrissageGood = true;
					break;
				}
				else
					break;
				break;

			default:
				break;
		}
		//AFFICHAGE
		//pour le defilement de la piste
		if (startindex < length - ACTOR_POS_X)
		{
			startindex++;
		}
		//affichage de la portion visible de la piste
		gotoxy(0, 6);
		if (startindex + ACTOR_POS_X < length)
		{
			for (int i = 0; i < ACTOR_POS_X; i++)
			{
				if (i + startindex > length)
					cout << " ";
				else
					cout << piste[i + startindex];
			}
		}
		else //quand on arrive au bout de la piste
		{
			// Effacer l'ancien avion
			gotoxy(plane->getX(), plane->getY());
			cout << "   ";
			plane->setX(plane->getX() + 1);
			//Print l'avion
			gotoxy(plane->getX(), plane->getY());
			cout << plane->getSprite();
		}

		//Affichage de l'avion
		gotoxy(plane->getX(), plane->getY());
		cout << plane->getSprite();

		//Affichage de la vitesse
		gotoxy(0, 7);
		cout << "Vitesse: " << speed << "          " << "Pointage: " << stat->getScore() << "    ";
		
		//Vitesse du jeu
		int delay = max(20, 100 / speed);
		Sleep(delay);

		//Condition d'echec
		if ((plane->getX() + startindex) >= length - 10)
		{
			system("cls");
			gotoxy(35, 3);
			cout << "L'avion s'est ecraser";
			stat->close;
			return false;
		}
	}
	return true;
}
*/
