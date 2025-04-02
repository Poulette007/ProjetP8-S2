#include "Game.h"
#define ACTOR_POS_X 1920
#define START_PLANE_X 5
Game::Game()
{
	listActor = vector<Actor*>();
	plane = new Plane(START_PLANE_X, 2);
	gameScene->addItem(plane);
	plane->show();
	stat = new Stat();
	isCollision = false;
	count = 0;
	possibleTouchDown = 500;
	msg_envoi["led"] = 1;
}

void Game::readKeyBoardGame()
{
	if (GetAsyncKeyState('W') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		stat->readKeybord('W');
	}
	if (GetAsyncKeyState('S') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		stat->readKeybord('S');
	}
	if (GetAsyncKeyState('A') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		stat->readKeybord('A');
	}
	if (GetAsyncKeyState('D') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		stat->readKeybord('D');
	}
}

void Game::update()
{
	int ch = 0;
	bool bpG = true;
	bool bpD = true;
	bool bpH = true;
	bool bpB = true;
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

	//Atterissage possible
	/*
	if (count >= possibleTouchDown && count <= (possibleTouchDown + 150))
	{
		gotoxy(30, 6);
		cout << "Atterissage possible, appuyez sur Gauche, ou k, pout initialiser la sequence datterissage!";
		if (_kbhit())
			ch = _getch();
		else if (ConnectionSerie::hasData())		//CA RUSH EN TABAROUETTE ICI, JE SAIS PAS PK
		{
			bpG = ConnectionSerie::getValue("BG");
			bpD = ConnectionSerie::getValue("BD");
			bpH = ConnectionSerie::getValue("BH");
			bpB = ConnectionSerie::getValue("BB");

		}
		if (ch == 'k' || bpG==0 || bpB==0||bpH==0||bpD==0)
		{
			stat->landing = true;
			stat->close = true;
		}
		if (count == possibleTouchDown + 150)
		{
			count = 0;
			possibleTouchDown += 150;
		}
	}*/
	count++;
}

void Game::manageCollision()
{
	for (int i = listActor.size() - 1; i >= 0; --i)
	{
		if (listActor[i]->x() <= plane->x() + plane->pixmap().width() + START_PLANE_X && listActor[i]->y() == plane->y())
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
	gotoxy(0, 6);
	int fuel = stat->getFuel();
	int score = stat->getScore();
	cout << "Gaz: " << fuel << endl;
	cout << "Vitesse: " << stat->getSpeed() << endl;
	cout << "Pointage: " << score << endl;
	cout << "Hauteur: " << stat->getHeight() << endl;

	//affiche gaz sur bargraph manette
	if (fuel>100)
	{
		msg_envoi["BAR"] = 10;
	}
	else if (fuel > 90 && fuel < 100)
	{
		msg_envoi["BAR"] = 9;
	}
	else if (fuel > 80 && fuel < 90)
	{
		msg_envoi["BAR"] = 8;
	}
	else if (fuel > 70 && fuel < 80)
	{
		msg_envoi["BAR"] = 7;
	}
	else if (fuel > 60 && fuel < 70)
	{
		msg_envoi["BAR"] = 6;
	}
	else if (fuel >50 && fuel < 60)
	{
		msg_envoi["BAR"] = 5;
	}
	else if (fuel > 40 && fuel < 50)
	{
		msg_envoi["BAR"] = 4;
	}
	else if (fuel > 30 && fuel < 40)
	{
		msg_envoi["BAR"] = 3;
	}
	else if (fuel > 20 && fuel < 30)
	{
		msg_envoi["BAR"] = 2;
	}
	else if (fuel > 10 && fuel < 20)
	{
		msg_envoi["BAR"] = 1;
	}
	else if (fuel<10)
	{
		msg_envoi["BAR"] = 0;
	}
	msg_envoi["LCD"] = "score: " + std::to_string(score);
	ConnectionSerie::Envoie(msg_envoi);
}

void Game::generateObstacles()
{
	int random = rand() % 2000;
	int posY = rand() % 3;
	bool tree = false;

	if (random >= 45 && random <= 50)
	{
		for (auto actor : listActor)
		{
			if (actor->y() == 3)
			{
				if (actor->x() >= (ACTOR_POS_X - 3))
					tree = true;
			}
		}
		if (!tree) {
			listActor.push_back(new Tree(ACTOR_POS_X, 3 * 1080 / 4));
			gameScene->addItem(listActor.back());
		}
			
	}
	if (isPosYPossible(posY))
	{
		int(posY *= 1080 / 4);
		if (random >= 1 && random <= 2) {
			listActor.push_back(new Wind(ACTOR_POS_X, posY));
			gameScene->addItem(listActor.back());
		}
		else if (random >= 3 && random <= 4) {
			listActor.push_back(new Gaz(ACTOR_POS_X, posY));
			gameScene->addItem(listActor.back());
		}
		else if (random >= 11 && random <= 12) {
			listActor.push_back(new Bird(ACTOR_POS_X, posY));
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
			if (abs(actor->x() - ACTOR_POS_X) < 700)
				return false;
		}
	}
	return true;
}
/*bool Game::takeoff()
{
	int speed = 1;
	int requiredSpeed = 100;
	int count = 0;
	int counttest = 0;
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
	vector<char> touches = { 'w','a','s','d'};
	vector<string> directions = { "haut", "gauche", "bas", "droite" };
	json msg_envoi;
	msg_envoi["led"] = 1;
	msg_envoi["gaz"] = 1;
	ConnectionSerie::Envoie(msg_envoi);
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
		bool hasD = ConnectionSerie::hasData();
		// --- Avec clavier ---	//
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
				cout << "Vitesse: " << speed << " / " << requiredSpeed;
			}
		}
		// --- Avec manette --- //
		else if (hasD == 1)
		{
			bool bpbas = ConnectionSerie::getValue("BB");
			if (!bpbas)
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
				cout << "Vitesse: " << speed << " / " << requiredSpeed;
			
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
			}
			bool hasD = ConnectionSerie::hasData();
			if (_kbhit())
			{
				if (_getch() == 32)
				{
					speed += 10;
				}
			}
			else if (hasD == 1)
			{
				bool bpB = ConnectionSerie::getValue("BB");
				if (!bpB)
				{
					speed += 10;
				}
			}
			if (speed <= 1)
				speed = 1;
			else if (speed >= requiredSpeed)
			{
				speed = requiredSpeed;
				vitesseGood = true;
				stat->changeFuel(200 - (startindex * 2));
			}
		}

		//phase 2 : decollage
		if (vitesseGood == true && hauteurGood == false)
		{
			//melange les touches
			gotoxy(15, 3);
			//todo : faire que l'atterissage fonctionne avec le clavier et la manette
			//cout << "APPUYEZ SUR " << touches[0] << " ECHEC --> " << conteurEchecHauteur << "/4" << " REUSSITE --> " << conteurReussiteHauteur << "/4";
			//if (_kbhit())
			//{
			//	touche = _getch();
			//	if (touche == touches[0])
			//	{
			//		conteurReussiteHauteur++;
			//	}
			//	else if (touche != touches[0] && touche != 32)
			//	{
			//		conteurEchecHauteur++;
			//	}
			//	shuffle(touches.begin(), touches.end(), random_device());
			//}
			cout << "APPUYEZ SUR " << directions[0] << " ECHEC --> " << conteurEchecHauteur << "/4" << " REUSSITE --> " << conteurReussiteHauteur << "/4";
			if (ConnectionSerie::hasData())
			{
				bool bphaut = ConnectionSerie::getValue("BH");
				bool bpgauche = ConnectionSerie::getValue("BG");
				bool bpdroite = ConnectionSerie::getValue("BD");
				bool bpbas = ConnectionSerie::getValue("BB");
				if (directions[0] == "haut")
				{
					if (!bphaut)
					{
						conteurReussiteHauteur++;
					}
					else if (!bpgauche || !bpdroite || !bpbas)
					{
						conteurEchecHauteur++;
					}
				}
				else if (directions[0] == "gauche")
				{
					if (!bpgauche)
					{
						conteurReussiteHauteur++;
					}
					else if (!bphaut || !bpdroite || !bpbas)
					{
						conteurEchecHauteur++;
					}
				}
				else if (directions[0] == "droite")
				{
					if (!bpdroite)
					{
						conteurReussiteHauteur++;
					}
					else if (!bphaut || !bpgauche || !bpbas)
					{
						conteurEchecHauteur++;
					}
				}
				else if (directions[0] == "bas")
				{
					if (!bpbas)
					{
						conteurReussiteHauteur++;
					}
					else if (!bphaut || !bpgauche || !bpdroite)
					{
						conteurEchecHauteur++;
					}
				}
				Sleep(30);	//todo : a enlever quand on aura debouncer les boutons
				shuffle(directions.begin(), directions.end(), random_device());
			}
			if (conteurReussiteHauteur >= 4)
			{
				hauteurGood = true;
				plane->setX(3);
			}
			else if (conteurEchecHauteur >= 4)
			{
				system("cls");
				gotoxy(0, 9);
				cout << "L'avion s'est ecrase";
				//Sleep(5000);
				return false;
			}
		}


		//AFFICHAGE
		gotoxy(0, 8);
		cout << "Vitesse: " << speed << " / " << requiredSpeed;
		if (speed == requiredSpeed)
		{
			gotoxy(0, 8);
			cout << "Vitesse: " << speed << " / " << requiredSpeed << " - SPEED OK!!";
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
			cout << "L'avion s'est ecarse";
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
					cout << "Activez la propultion invese en appuiant sur: P: OK!";
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
