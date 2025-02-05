#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include "Actor.h"
#include "Obstacle.h"
#include "Plane.h"

using namespace std;


class Game
{
public:
	Game();
	vector<Actor*> listActor;
	void generateObstacles();
	void update();
	void manageCollision();
	void CollionDetected(Actor* actor);
	int framecounter;
	bool isCollision;
	void gotoxy(int x, int y);
	Plane *plane;
	
};

