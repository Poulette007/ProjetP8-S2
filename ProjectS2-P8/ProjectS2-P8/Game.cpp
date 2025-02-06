#include "Game.h"
#define POS_X 100
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

	gotoxy(plane->getX(), stat->getHeight());
	plane->setY(stat->getHeight());
	cout << plane->getSprite();

	afficherStat();
	manageCollision();
	generateObstacles();
}

void Game::manageCollision()
{
	for (auto actor : listActor)
	{
		if (actor->getX() <= plane->getX() + plane->getLength() + START_PLANE_X && actor->getY() == plane->getY())
			CollionDetected(actor);

	}
}
void Game::CollionDetected(Actor* actor)
{
	cout << "Collion detected";
	isCollision = true;
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
	int random = rand() % 100;
	int posY = rand() % 4;
	if (isPosYPossible(posY))
	{
		switch (random)
		{
		case 1: {

			listActor.push_back(new Wind(POS_X, posY));
			break;
		}
		case 2: {
			listActor.push_back(new Gaz(POS_X, posY));
			break;
		}
		case 3: {
			listActor.push_back(new Tree(POS_X, posY));
			break;
		}
		case 4: {
			listActor.push_back(new Bird(POS_X, posY));	
			break;
		}
		default:
			break;
		}

	}
}
bool Game::isPosYPossible(int y)
{
    int nombre = 0;
    for (auto actor : listActor)
    {
        if (abs(actor->getX() - POS_X) < 30 && actor->getY() != y)
            nombre++;
    }
    return nombre < 2;
}