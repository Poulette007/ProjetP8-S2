#pragma once
#include "Actor.h"
class Plane : public Actor
{
public:
	Plane(int x, int y, int length, std::string sprite, int speed);
	void action() override;
	int getSpeed() const override;
private:
	int speed;
};

