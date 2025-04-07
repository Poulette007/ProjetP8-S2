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
#include "include/json.hpp""
#include "SmallTexte.h"
#include "GameOver.h"
#include <qlayout.h>
#include <QDashboard.h>
#include <QStackedWidget>
using namespace std;
class Takeoff; // évite l'inclusion circulaire
class Landing; // évite l'inclusion circulaire

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
	enum class Gamestate
	{
		Decollage,
		Gameplay,
		Landing,
		GameOver
	};

	Game(Stat* s, QStackedWidget* stack, GameOver* gameOverPage);
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
	void afficherStatManette();
	void afficherStatOnGame();
	void setupStatOnGame();
	Plane *plane;
	Takeoff* takeoff;
	Landing* landing;
	Stat* stat;
	QGraphicsTextItem* promptText;
	Gamestate state;
	QStackedWidget* s;
	GameOver* gameOver;
	int count;
	//int possibleTouchDown;
	bool isCollision;
	int landingCount;
	
};

