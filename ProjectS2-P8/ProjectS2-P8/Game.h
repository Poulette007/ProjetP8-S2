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
class Takeoff; // �vite l'inclusion circulaire
class Landing; // �vite l'inclusion circulaire

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
	void generateObstacles();
	bool isPosYPossible(int y);
	void update();
	void updateGameplay();
	void manageCollision();
	void CollionDetected(Actor* actor);
	void gotoxy(int x, int y);
	void afficherStat();
	bool possibleLanding();

	vector<Actor*> listActor;
	Plane *plane;
	Takeoff* takeoff;
	Landing* landing;
	Stat* stat;
	QGraphicsTextItem* promptText;
	Gamestate state;

	int count;
	//int possibleTouchDown;
	bool isCollision;
	int landingCount;
};

