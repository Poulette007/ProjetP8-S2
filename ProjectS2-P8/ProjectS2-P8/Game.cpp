#include "Game.h"
#define ACTOR_POS_X 100
#define START_PLANE_X 5
Game::Game()
{
	listActor = vector<Actor*>();
	plane = new Plane(START_PLANE_X, 2);
	stat = new Stat();
	isCollision = false;
	count = 0;
	possibleTouchDown = 500;
}
void Game::update()
{
	int ch = 0;
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

	//Atterissage possible
	if (count >= possibleTouchDown && count <= (possibleTouchDown + 200))
	{
		gotoxy(30, 6);
		cout << "Atterissage possible, appuyez sur K!";
		if (_kbhit())
			ch = _getch();
		if (ch == 'k')
		{
			stat->landing = true;
			stat->close = true;
		}
		if (count == possibleTouchDown + 200)
		{
			count = 0;
			possibleTouchDown += 500;
		}
	}
	count++;
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
	cout << "Gaz: " << stat->getFuel() << endl;
	cout << "Vitesse: " << stat->getSpeed() << endl;
	cout << "Pointage: " << stat->getScore() << endl;
	cout << "Hauteur: " << stat->getHeight() << endl;
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
				cout << "Diminuez la vitesse de 50%";
				if (_kbhit())
					ch = _getch();
				else
					break;
				if (ch == 'a')
				{
					speed -= 5;
					ch = 0;
				}
				else
					break;
				if (speed <= 50)
				{
					state = 1;
					gotoxy(0, 8);
					cout << "Diminuez la vitesse de 50%: OK!";
				}
				else
					break;
				break;
			case 1: //Diminuer la hauteur de l'avion
				gotoxy(0, 9);
				cout << "Diminuez la hauteur de 2 positions";
				if (_kbhit())
					ch = _getch();
				if (ch == 's')
				{
					gotoxy(plane->getX(), plane->getY());
					cout << "      "; //Supprimer l'avion pour la deplacer
					plane->setY(plane->getY() + 1);
					ch = 0;
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
				cout << "Sortez le train d'atterissage en appuiant sur: T";
				if (_kbhit())
					ch = _getch();
				if (ch == 't')
				{
					gotoxy(0, 10);
					cout << "Sortez le train d'atterissage en appuiant sur: T: OK!";
					state = 3;
				}
				else
					break;
				break;
			case 3:
				gotoxy(0, 11);
				cout << "Diminuez la vitesse de 25%";
				if (_kbhit())
					ch = _getch();
				else
					break;
				if (ch == 'a')
				{
					speed -= 1;
					ch = 0;
				}
				else
					break;
				if (speed <= 25)
				{
					state = 4;
					gotoxy(0, 11);
					cout << "Diminuez la vitesse de 25%: OK!";
				}
				else
					break;
				break;
			case 4:
				gotoxy(0, 12);
				cout << "Touchez le sol";
				if (_kbhit())
					ch = _getch();
				if (ch == 's')
				{
					gotoxy(plane->getX(), plane->getY());
					cout << "      ";
					plane->setY(plane->getY() + 1);
					ch = 0;
				}
				else
					break;
				if (plane->getY() == 5)
				{
					gotoxy(0, 12);
					cout << "Touchez le sol: OK!";
					state = 5;
				}
				else
					break;
				break;
			case 5:
				gotoxy(0, 13);
				cout << "Diminuez la vitesse a 10%";
				if (_kbhit())
					ch = _getch();
				else
					break;
				if (ch == 'a')
				{
					speed -= 1;
					ch = 0;
				}
				else
					break;
				if (speed <= 10)
				{
					state = 6;
					gotoxy(0, 13);
					cout << "Diminuez la vitesse a 10%: OK!";
				}
				else
					break;
				break;
			case 6:
				gotoxy(0, 14);
				cout << "Activez la propultion invese en appuiant sur: P";
				if (_kbhit())
					ch = _getch();
				if (ch == 'p')
				{
					gotoxy(0, 14);
					cout << "Activez la propultion invese en appuiant sur: P: OK!";
					state = 7;
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
				cout << "Immobiliser l'avion avec les freins en appuiant sur: F";
				if (_kbhit())
					ch = _getch();
				if (ch == 'f')
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
			cout << "Plane crashed";
			stat->close;
			return false;
		}
	}
	return true;
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