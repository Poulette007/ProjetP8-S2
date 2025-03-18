#include "Stat.h"
#include "const.h"

#define PALLIER_1 0
#define PALLIER_2 1
#define PALLIER_3 2
#define PALLIER_4 3

#define MONTER_1_PALLIER -1
#define DESCENDRE_1_PALLIER 1



int Stat::fuel = 0;
int Stat::speed = 0;
int Stat::score = 0;
int Stat::height = 0;
bool Stat::close = false;
bool Stat::landing = false;

Stat::Stat()
{
	
	changeFuel(100);
	changeSpeed(MIN_SPEED); //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
	changeScore(0);
	changeHeight(PALLIER_2); //Hauteur pour debut quand en vol (nombres semi-bullshit)
	delay = 0;
}

Stat::Stat(int scoreIni)
{
	changeFuel(100);
	changeSpeed(MIN_SPEED); //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
	changeScore(scoreIni);
	changeHeight(PALLIER_2); //Hauteur pour debut quand en vol (nombres semi-bullshit)
	delay = 0;
}


void Stat::changeFuel(int F)
{
	if ((fuel + F) <= 0)
	{
		close = true;
	}
	else if ((fuel + F) > MAX_FUEL)
	{
		fuel = MAX_FUEL;
	}
	else {
		fuel += F;
	}
}

int Stat::getFuel()
{
	return fuel;
}

void Stat::changeSpeed(int Sp)
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

void Stat::changeScore(int Sc)
{
	if (Sc > 0)
		score += Sc;
}

int Stat::getScore()
{
	return score;
}

void Stat::changeHeight(int H)
{
	if (height + H >= PALLIER_1 && height + H <= PALLIER_4)
	{
		height += H;
	}
}

void Stat::countFuel()
{
	delay++;
	if (delay >= 50)
	{
		delay = 0;
		changeFuel(-10);
	}
}


int Stat::getHeight()
{
	return height;
}

void Stat::readKeybord()
{
	int pot = 0;
	int joy_haut = 0;
	int joy_bas = 0;
	int ch = 0;
	if (_kbhit())
		ch = _getch();

	if (connection.hasData())
	{
		pot = connection.getValue("pot");
		joy_haut = connection.getValue("JH");
		joy_bas = connection.getValue("JB");
	}
	if (joy_haut == 1)
	{
		changeHeight(MONTER_1_PALLIER);
	}
	if (joy_bas == 1)
	{
		changeHeight(DESCENDRE_1_PALLIER);
	}
	
	switch (ch)
	{
	case 'w': //w (monter en altitude)
		changeHeight(MONTER_1_PALLIER);
		break;

	case 's': //s (diminu l'altitude)
		changeHeight(DESCENDRE_1_PALLIER);
		break;
	case 'a':
		changeSpeed(-1);
		break;
	case 'd':
		changeSpeed(1);
		break;
	default:
		break;
	}
}

