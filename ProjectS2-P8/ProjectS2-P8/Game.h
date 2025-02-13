#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <random>
#include <algorithm>

#include "Actor.h"
#include "Obstacle.h"
#include "Plane.h"
#include "Stat.h"
#include "Player.h"
using namespace std;


class Game
{
public:
	Game();
	vector<Actor*> listActor;
	void generateObstacles();
	bool takeoff();
	bool isPosYPossible(int y);
	void update();
	void manageCollision();
	void CollionDetected(Actor* actor);
	bool isCollision;
	void gotoxy(int x, int y);
	void afficherStat();
	Plane *plane;
	Stat* stat;
	
};

