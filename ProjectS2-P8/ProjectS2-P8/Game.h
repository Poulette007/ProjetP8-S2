#pragma once
#include <string>
#include <vector>
#include <iostream>
#include <windows.h>
#include <random>
#include <algorithm>
#include <QSlider.h>

#include "Actor.h"
#include "Obstacle.h"
#include "Plane.h"
#include "Stat.h"
#include "Player.h"
#include "ConnectionSerie.h"
#include <qlayout.h>
#include <QDashboard.h>
using namespace std;


class Game
{
private: 
	json msg_envoi;
	QGraphicsPixmapItem* looseBackGround;
	QGraphicsPixmapItem* BackGroundVol;

	bool startLoose = true;
	// Stat QT 
	QDashboard *dash;
public:
	Game(Stat*);
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
	void afficherStatManette();
	void afficherStatOnGame();
	void setupStatOnGame();
	Plane *plane;
	Stat* stat;
	int count;
	int possibleTouchDown;
};

