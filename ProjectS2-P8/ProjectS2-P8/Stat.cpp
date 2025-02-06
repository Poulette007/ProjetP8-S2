#include "Stat.h"
#define PALLIER_1 0
#define PALLIER_2 1
#define PALLIER_3 2
#define PALLIER_4 3

#define MONTER_1_PALLIER -1
#define DESCENDRE_1_PALLIER 1

#define MAX_SPEED 100
#define MIN_SPEED 50
#define MAX_FUEL 100
#define MIN_FUEL 0


Stat::Stat()
{
	fuel = 0;
	speed = MIN_SPEED; //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
	score = 0;
	height = PALLIER_2; //Hauteur pour debut quand en vol (nombres semi-bullshit)
}

Stat::Stat(int score)
{
	fuel = 0;
	speed = MIN_SPEED;
	this->score = score;
	height = PALLIER_2;
}


void Stat::setFuel(int F)
{
	if ((fuel + F <= MAX_FUEL) && (fuel + F >= MIN_FUEL))
	{
		fuel += F;
	}
}

int Stat::getFuel()
{
	return fuel;
}

void Stat::setSpeed(int Sp)
{
	if ((speed + Sp <= MAX_SPEED) && (speed + Sp >= MIN_SPEED))
	{
		speed += Sp;
	}
}

int Stat::getSpeed()
{
	return speed;
}

void Stat::setScore(int Sc)
{
	if (Sc > 0)
		score += Sc;
}

int Stat::getScore()
{
	return score;
}

void Stat::setHeight(int H)
{
	if (height + H >= PALLIER_1 && height + H <= PALLIER_4)
	{
		height += H;
	}
}


int Stat::getHeight()
{
	return height;
}

void Stat::readKeybord()
{
	int ch = 0;
	if (_kbhit())
		ch = _getch();

	switch (ch)
	{
	case 'w': //w (monter en altitude)
		setHeight(MONTER_1_PALLIER);
		break;

	case 's': //s (diminu l'altitude)
		setHeight(DESCENDRE_1_PALLIER);
		break;
	default:
		break;
	}
}

void Stat::changeFuel(int fuel)
{
	setFuel(getFuel() + fuel);
}
