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
	
	QGraphicsPixmapItem* looseBackGround;
	GameOver* overGame;
	QStackedWidget* stack;
	bool startLoose = true;
	// Stat QT 
	QDashboard *dash;

	int random;
	int obstacle;
	int posY;
	int distance;
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
	void createObstacle(int obstacle, int posY);
	bool isPosYPossible(int y);
	void update();
	void updateGameplay();
	void changePlanePixmap();
	void manageCollision();
	void CollionDetected(Actor* actor);
	void gotoxy(int x, int y);
	void afficherStat();
	bool possibleLanding();

	vector<Actor*> listActor;
	void afficherStatManette();
	void etteinManette();
	void afficherStatOnGame();
	void setupStatOnGame();
	json msg_envoi;
	Plane *plane;
	Takeoff* takeoff;
	Landing* landing;
	Stat* stat;
	FormatTextPixmap* promptText;
	Gamestate state;
	QStackedWidget* s;
	GameOver* gameOver;
	int count;
	//int possibleTouchDown;
	bool isCollision;
	int landingCount;
};

