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
#include "ConnectionSerie.h"
using namespace std;


class Game
{
private: 
	json msg_envoi;
public:
	Game();
	void readKeyBoardGame();
	vector<Actor*> listActor;
	void generateObstacles();
	//bool takeoff();
	//bool touchDown();
	bool isPosYPossible(int y);
	void update();
	void manageCollision();
	void CollionDetected(Actor* actor);
	bool isCollision;
	void gotoxy(int x, int y);
	void afficherStat();
	Plane *plane;
	Stat* stat;
	int count;
	int possibleTouchDown;
};

