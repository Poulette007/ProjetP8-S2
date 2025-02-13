#include "Game.h"
#define ACTOR_POS_X 100
#define START_PLANE_X 5
Game::Game()
{
	listActor = vector<Actor*>();
	plane = new Plane(START_PLANE_X, 2);
	stat = new Stat();
	isCollision = false;
}
void Game::update()
{
	for (auto actor : listActor)
	{
		gotoxy(actor->getX(), actor->getY());
		cout << actor->getSprite();
		actor->setX(actor->getX() - actor->getSpeed());
		if (actor->getX() <= START_PLANE_X - actor->getLength())
		{
			listActor.erase(std::remove(listActor.begin(), listActor.end(), actor), listActor.end());
		}
	}
	stat->readKeybord();

	stat->changeScore(250);
	stat->countFuel();

	gotoxy(plane->getX(), stat->getHeight());
	plane->setY(stat->getHeight());
	cout << plane->getSprite();

	afficherStat();
	manageCollision();
	generateObstacles();
}

void Game::manageCollision()
{
	for (int i = 0; i < listActor.size(); i++)
	{
		if (listActor[i]->getX() <= plane->getX() + plane->getLength() + START_PLANE_X && listActor[i]->getY() == plane->getY()) {
			CollionDetected(listActor[i]);
			listActor.erase((listActor.begin() + i));
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
	gotoxy(0, 6);
	cout << "Fuel : " << stat->getFuel() << endl;
	cout << "Speed : " << stat->getSpeed() << endl;
	cout << "Score : " << stat->getScore() << endl;
	cout << "Height : " << stat->getHeight() << endl;
}

void Game::generateObstacles()
{
	int random = rand() % 200;
	int posY = rand() % 3;
	bool tree = false;

	if (random >= 45 && random <= 50)
	{
		for (auto actor : listActor)
		{
			if (actor->getY() == 3)
			{
				if (actor->getX() >= (ACTOR_POS_X - 3))
					tree = true;
			}
		}
		if (!tree)
			listActor.push_back(new Tree(ACTOR_POS_X, 3));
	}
	if (isPosYPossible(posY))
	{
		if (random >= 1 && random <= 2) {
			listActor.push_back(new Wind(ACTOR_POS_X, posY));
		}
		else if (random >= 3 && random <= 4) {
			listActor.push_back(new Gaz(ACTOR_POS_X, posY));
		}
		else if (random >= 11 && random <= 12) {
			listActor.push_back(new Bird(ACTOR_POS_X, posY));
		}
	}
}
bool Game::takeoff()
{
	int speed = 1;
	int requiredSpeed = 100;
	int count = 0;
	int startindex = 0;
	int conteurReussiteHauteur = 0;
	int conteurEchecHauteur = 0;
	char touche = ' ';
	plane->setX(0);
	plane->setY(5);
	int length = rand() % 200 + 300;
	bool vitesseGood = false;
	bool hauteurGood = false;
	vector<char> piste(length);
	vector<char> touches = { 'w','a','s','d' };
	//on remplit la piste
	for (int i = 0; i < length; i++)
	{
		if (i % 10 == 1)
			piste[i] = '|';
		else if (i >= length - 5)
			piste[i] = 'X';
		else
			piste[i] = '-';
	}

	//affichage initiale de la piste
	gotoxy(0, 5);
	cout << plane->getSprite();
	gotoxy(0, 6);
	for (int i = 0; i < ACTOR_POS_X; i++)
	{
		cout << piste[i];
	}
	//petit mouvement initiale
	while (plane->getX() < START_PLANE_X)
	{
		if (_kbhit())
		{
			if (_getch() == 32)
			{
				// Effacer l'ancien avion
				gotoxy(plane->getX(), plane->getY());
				cout << "   ";
				speed++;
				stat->changeSpeed(speed);

				plane->setX(plane->getX() + 1);
				gotoxy(plane->getX(), plane->getY());
				cout << plane->getSprite();

				gotoxy(0, 8);
				cout << "Speed : " << speed << " / " << requiredSpeed;
			}
		}
	}

	while (!vitesseGood || !hauteurGood)
	{
		//phase 1 : acceleration
		if (!vitesseGood)
		{
			count++;
			if (count % 2 == 0)
			{
				speed -= 1;
				//stat->setSpeed(speed);
			}
			if (_kbhit())
			{
				if (_getch() == 32)
				{
					speed += 10;
					//stat->setSpeed(speed);
				}
			}
			if (speed <= 1)
				speed = 1;
			else if (speed >= requiredSpeed)
			{
				speed = requiredSpeed;
				vitesseGood = true;
				stat->changeFuel(1000 - (startindex * 2));
			}
		}

		//phase 2 : decollage
		if (vitesseGood == true && hauteurGood == false)
		{
			//melange les touches
			gotoxy(15, 3);
			cout << "APPUYEZ SUR " << touches[0] << " ECHEC --> " << conteurEchecHauteur << "/4" << " REUSSITE --> " << conteurReussiteHauteur << "/4";
			if (_kbhit())
			{
				touche = _getch();
				if (touche == touches[0])
				{
					conteurReussiteHauteur++;
				}
				else if (touche != touches[0] && touche != 32)
				{
					conteurEchecHauteur++;
				}
				shuffle(touches.begin(), touches.end(), random_device());
			}
			if (conteurReussiteHauteur >= 4)
			{
				hauteurGood = true;
			}
		}


		//AFFICHAGE
		gotoxy(0, 8);
		cout << "Speed : " << speed << " / " << requiredSpeed;
		if (speed == requiredSpeed)
		{
			gotoxy(0, 8);
			cout << "Speed : " << speed << " / " << requiredSpeed << " - SPEED OK!!";
		}
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
			gotoxy(plane->getX(), plane->getY());
			cout << plane->getSprite();

		}

		int delay = max(20, 100 / speed);
		Sleep(delay);

		//condition d'echec
		if ((plane->getX() + startindex) >= length - 10)
		{
			system("cls");
			gotoxy(0, 9);
			cout << "plane crashed";
			stat->close;
			//sSleep(5000);

			return false;
		}
	}
}


bool Game::isPosYPossible(int y)
{
		for (auto actor : listActor)
		{
			if (actor->getY() == y) 
			{
				if (abs(actor->getX() - ACTOR_POS_X) < 20)
					return false;
			}
		}
		return true;
}