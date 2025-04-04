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
#include "include/json.hpp""
#include "SmallTexte.h"

using namespace std;
class Takeoff; // évite l'inclusion circulaire

class Game
{
private: 
	json msg_envoi;

public:
	enum class Gamestate
	{
		Decollage,
		Gameplay,
		Landing,
		GameOver
	};

	Game();
	void readKeyBoardGame();
	vector<Actor*> listActor;
	void generateObstacles();
	//bool takeoff();
	//bool touchDown();
	bool isPosYPossible(int y);
	void update();
	void updateGameplay();
	void manageCollision();
	void CollionDetected(Actor* actor);
	bool isCollision;
	void gotoxy(int x, int y);
	void afficherStat();
	//void setState(Gamestate);
	Plane *plane;
	Takeoff* takeoff;
	Stat* stat;
	int count;
	int possibleTouchDown;
	QGraphicsTextItem* promptText;
	Gamestate state;

};

