#pragma once
#include "Actor.h"
#include "ImageManager.h"

class Plane : public Actor
{
public: 
	Plane(int x, int y, const QPixmap& pixmap, int speed);
	Plane(int x, int y);
	void action(Stat stat) override;
	int getSpeed() const override;
	void setY(int y);
private:
	int speed;
};

