#include "Game.h"

Game::Game()
{
	listActor = vector<Actor*>();
	plane = new Plane(1, 1, 5, "ooo", 0);
	framecounter = 0;
	isCollision = false;
}
void Game::update()
{
	framecounter++;
	if (framecounter % 3 == 0)
	{
		framecounter = 0;
	}
	for (auto actor : listActor)	//pour chaque obstacle dans obstacle
	{
		gotoxy(actor->getX() - actor->getSpeed(), actor->getY()); // TODO : check for + or * QT speed
		actor->setX(actor->getX() - actor->getSpeed());
		cout << actor->getSprite();
	}
	gotoxy(plane->getX(), plane->getY());
	cout << plane->getSprite();

	manageCollision();
	generateObstacles();
}

void Game::manageCollision()
{
	for (auto actor : listActor)
	{
		if (actor->getX() <= plane->getX() + plane->getLength() && actor->getY() == plane->getY())
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

void Game::generateObstacles()
{
	int random = rand() % 10;
	switch (random)
	{
	case 1: {
		Wind *wind = new Wind(100, 2 ,3 , "~~~",2);
		listActor.push_back(wind);
		break;
	}
	case 2: {
		Gaz * gaz = new Gaz(200, 10, 1, "@", 2);
		listActor.push_back(gaz);
		break;
	}
	default:
		break;
	}
}