#pragma once
#include "Game.h"
class Takeoff : public QObject
{
	Q_OBJECT
public:
	Takeoff(Game* game, Plane* p, Stat* s, FormatTextPixmap* prompt, QStackedWidget* stack, GameOver* gameOverPage);
	void initPiste();
	void updateTakeoff();
	int readInputDecollage();
	void shuffleDirection();
	void animationTakeoff();
public slots:
	void updateAccelerationInitiale();
	void updateAcceleration();
	void updateHeight();
private:
	QTimer* takeoffTimer;
	vector<QGraphicsPixmapItem*> runwayTilePixmap;
	enum class TakeoffPhase
	{
		AccelerationInitiale,
		Acceleration,
		Height,
		Success,
		Failure
	};
	TakeoffPhase takeoffPhase = TakeoffPhase::AccelerationInitiale;
	Plane* plane;
	Stat* stat;
	FormatTextPixmap* promptText;
	QGraphicsPixmapItem* tower;
	QGraphicsPixmapItem* airport;
	string directionPrompt;
	Game* gameref;
	QStackedWidget* stack;
	GameOver* gameOver;
	int countRalentissement;
	int longeurPiste;
	int startindex=0;
	int successCount=0;
	int failCount = 0;
	int inputCount;
	int input;
	int speed=0;
	int recquiredSpeed = 200;
	bool upPlane = true;
};

