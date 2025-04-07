#pragma once
#include "Game.h"
class Landing : public QObject
{
	Q_OBJECT
public:
	Landing(Game* game, Plane* p, Stat* s, QGraphicsTextItem* prompt);
	void initPiste();
	void updateLanding();
	int readInputAtterrissage();
public slots:
	void updateRalentissement();
	void updateDescente();
	void updateTrainAtterrissage();
	void updateAtterrissage();
private: 
	enum class LandingPhase
	{
		Ralentissement,
		Descente,
		TrainAtterrissage,
		Atterissage,
		Success,
		Failure
	};
	LandingPhase landingPhase = LandingPhase::Ralentissement;
	QTimer* landingTimer;
	vector<QGraphicsPixmapItem*> runwayTilePixmap;
	Plane* plane;
	Stat* stat;
	QGraphicsTextItem* promptText;
	Game* gameref;

	int input=0;
	int longeurPiste;
	int speed = 100;
	bool trainSorti = false;
};

