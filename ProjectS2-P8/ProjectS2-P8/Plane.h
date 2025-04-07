#pragma once
#include "Actor.h"
#include "ImageManager.h"

class Plane : public Actor
{
public:
	Plane(int x, int y);
	void action(Stat stat) override;
	int getSpeed() const override;
	void changePixmap(int skin);
private:
	int speed;
};

