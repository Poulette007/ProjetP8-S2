#include "Plane.h"
Plane::Plane(int x, int y, int length, std::string sprite, int speed)
    : Actor(x, y, length, sprite), speed(speed) {
}
Plane::Plane(int x, int y) : Actor(x, y)
{
	speed = 0;
	setSprite("::^=^>");
}
void Plane::action()  {
    std::cout << "Obstacle -- plane action()" << std::endl;
}
int Plane::getSpeed() const {
    return speed; 
}
void Plane::setSprite(std::string sprite) {
	this->sprite = sprite;
}

void Plane::setY(int y)
{
	this->posY = y;
}
