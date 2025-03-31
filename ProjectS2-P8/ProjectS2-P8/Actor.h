#pragma once

#include <string>
#include <iostream>

#include "Stat.h"
#include <qpixmap.h>
class Actor
{
public: 	
	// Avec QPixmap
	Actor(int x, int y, const QPixmap& pixmap);
	Actor(int x, int y);
	
	virtual void action(Stat stat) = 0;
	//virtual void setPixmap(const QPixmap& pixmap) = 0;
	virtual int getSpeed() const = 0;


	void afficher() const;
	void setX(int x);
	int getX();
	int getY();
	int getLength();
	QPixmap getPixmap() const;
	std::string getSprite();
protected:
	int length;
	int posY;
	int posX;
	std::string sprite;
	QPixmap pixmap;
	void setPosition(int x, int y, int  lenght);
};