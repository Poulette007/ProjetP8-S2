//#include "Stat.h"
//#define PALLIER_1 1
//#define PALLIER_2 2
//#define PALLIER_3 3
//#define PALLIER_4 4
//
//#define MONTER_1_PALLIER 1
//#define DESCENDRE_1_PALLIER -1
//
//Stat::Stat()
//{
//	fuel = 0;
//	speed = 200; //Vitesse de base quand en vol (je bullshit des nombres pour l'instant)
//	score = 0;
//	height = 1000; //Hauteur pour debut quand en vol (nombres semi-bullshit)
//}
//
//Stat::~Stat()
//{
//}
//
//void Stat::setFuel(int F)
//{
//	if (F + fuel >= 100) //Depasse pas le 100%
//	{
//		fuel = 100;
//		return;
//	}
//	else if (fuel - F <= 0) //Depasse pas le 0% 
//	{
//		fuel = 0;
//		return;
//	}
//	else
//		fuel += F;
//	return;
//}
//
//int Stat::getFuel()
//{
//	return fuel;
//}
//
//void Stat::setSpeed(int Sp)
//{
//	if (speed + Sp >= 2000) //Depasse pas 2000km/h
//	{
//		speed = 2000;
//		return;
//	}
//	else if (speed - Sp <= 0) //Depasse pas le 0km/h
//	{
//		speed = 0;
//		return;
//	}
//	else
//		speed += Sp;
//	return;
//}
//
//int Stat::getSpeed()
//{
//	return speed;
//}
//
//void Stat::setScore(int Sc)
//{
//	if (score - Sc <= 0) //Depasse pas 0 
//	{
//		score = 0;
//		return;
//	}
//	else
//		score += Sc; //peut monter vraiement haut, pas de cap
//	return;
//}
//
//int Stat::getScore()
//{
//	return score;
//}
//
//void Stat::setHeight(int H)
//{
//	/*
//	Hauteur minimal est de 120m (hauteur tour radio)
//	Premier palier est a 2476m
//	2ieme 4752m
//	3ieme 7248
//	4ieme 9624
//	5ieme 12000 (hauteur max vol commercial) chat cap*/ 
//
//	if (height + H >= PALLIER_4) //Depasse pas le 12000m
//	{
//		height = PALLIER_4;
//		return;
//	}
//	else if (height + H <= PALLIER_1) //Depasse pas 120m
//	{
//		height = PALLIER_1;
//		return;
//	}
//	else
//		height += H;
//}
//
//int Stat::getHeight()
//{
//	return height;
//}
//
//void Stat::readKeybord()
//{
//	int ch = 0;
//	if (kbhit())
//		ch = getch();
//
//	switch (ch)
//	{
//	case 'w': //w (monter en altitude)
//		setHeight(MONTER_1_PALLIER);
//		break;
//
//	case 's': //s (diminu l'altitude)
//		setHeight(DESCENDRE_1_PALLIER);
//		break;
//	default:
//		break;
//	}
//}
//
//void Stat::changeFuel(int fuel)
//{
//	setFuel(getFuel() + fuel);
//}
