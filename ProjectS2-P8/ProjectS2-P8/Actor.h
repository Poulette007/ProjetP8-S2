#pragma once

#include <string>
#include <iostream>

#include "Stat.h"
class Actor
{
public: 
	Actor(int x, int y, int lenght, std::string);
	Actor(int x, int y);
	virtual void action(Stat stat) = 0;
	virtual void setSprite(std::string sprite) = 0;
	//virtual bool canSpawn(int y) = 0;
	virtual int getSpeed() const = 0;


	void afficher();
	void setX(int x);
	int getX();
	int getY();
	int getLength();
	std::string getSprite();
protected:
	int length;
	int posY;
	int posX;
	std::string sprite;
	void setPosition(int x, int y, int  lenght);
};