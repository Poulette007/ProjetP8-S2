#include "Stat.h"
#include "const.h"

#define PALLIER_1 0
#define PALLIER_2 1 * 1080/4
#define PALLIER_3 2 * 1080/4
#define PALLIER_4 3 * 1080/4

#define MONTER_1_PALLIER -1080/4
#define DESCENDRE_1_PALLIER 1080/4


int Stat::fuel = 0;
int Stat::speed = 0;
int Stat::score = 0;
int Stat::height = 0;
int Stat::speedfactor = 0;
bool Stat::close = false;
bool Stat::landing = false;
bool Stat::muonTrue = false;
int Stat::skinPlane = 0;
QString Stat::playerName = "Player";
int Stat::previousScore = 0;

Stat::Stat()
{
	
	changeFuel(10);
	changeSpeed(100/8); //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
	changeScore(0);
	changeHeight(PALLIER_4); //Hauteur pour debut quand en vol (nombres semi-bullshit)
	delay = 0;
}

Stat::Stat(int scoreIni)
{
	changeFuel(10);
	changeSpeed(MIN_SPEED); //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
	changeScore(scoreIni);
	changeHeight(PALLIER_4); //Hauteur pour debut quand en vol (nombres semi-bullshit)
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
		speedfactor = speed / 3.0f;
		if (Sp > 0)
		{
			changeFuel(-1);
		}
	}
}
void Stat::setSpeed(int Sp)
{
	if (Sp >= MIN_SPEED && Sp <= MAX_SPEED)
	speed = Sp;
}

int Stat::getSpeed()
{
	return speed;
}

void Stat::changeScore(int Sc)
{
	if (Sc > 0)
		score += Sc * speed;
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

		changeFuel(-1);
	}
}


int Stat::getHeight()
{
	return height;

}
int Stat::getHeightRevers()
{
	if (height == PALLIER_1)
	{
		return PALLIER_4;
	}
	else if (height == PALLIER_2)
	{
		return PALLIER_3;
	}
	else if (height == PALLIER_3)
	{
		return PALLIER_2;
	}
	else if (height == PALLIER_4)
	{
		return PALLIER_1;
	}
}
void Stat::readManette(int pot)
{
	/*
	int pot = 0;
	int joy_haut = 0;
	int joy_bas = 0;

	if (ConnectionSerie::hasData())
	{
		pot = ConnectionSerie::getValue("pot");
		joy_haut = ConnectionSerie::getValue("JH");
		joy_bas = ConnectionSerie::getValue("JB");
	}

	//section pour le controle de la hauteur par le baton de joi
	if (joy_haut == 1)
	{
		changeHeight(MONTER_1_PALLIER);
	}
	if (joy_bas == 1)
	{
		changeHeight(DESCENDRE_1_PALLIER);
	}*/
	//section pour le controle de la vitesse par le pots
	if (pot > 0 && pot < 134)
	{
		setSpeed(MIN_SPEED);
	}
	else if (pot > 134 && pot < 268)
	{
		setSpeed(5);
	}
	else if (pot > 268 && pot < 402)
	{
		setSpeed(10);
	}
	else if (pot > 402 && pot < 536)
	{
		setSpeed(15);
	}
	else if (pot > 536 && pot < 670)
	{
		setSpeed(25);
	}
}

void Stat::readKeybord(char key)
{

	switch (key)
	{
	case 'W':
		changeHeight(MONTER_1_PALLIER);
		std::cout << "w";
		break;

	case 'S':
		changeHeight(DESCENDRE_1_PALLIER);
		break;
	case 'A':
		changeSpeed(-1);
		break;
	case 'D':
		changeSpeed(1);
		break;
	default:
		break;
	}
}

