#pragma once
#include <conio.h>
#include <iostream>
class Stat
{
private:
	static int fuel;
	static int speed;
	static int score;
	static int height;
	int delay;

public:
	static bool close;
	static bool landing;
	Stat();
	Stat(int score);
	static int getFuel();
	static int getSpeed();
	static int getScore();
	static int getHeight();
	void readKeybord();
	static void changeFuel(int F);
	static void changeSpeed(int Sp);
	static void changeScore(int Sc);
	static void changeHeight(int H);
	void countFuel();
};
