#pragma once
#include "Actor.h"
class Plane : public Actor
{
public:
	Plane(int x, int y, int length, std::string sprite, int speed);
	Plane(int x, int y);
	void action(Stat stat) override;
	int getSpeed() const override;
	void setSprite(std::string sprite) override;
	void setY(int y);
private:
	int speed;
};

