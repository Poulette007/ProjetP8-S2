#include "Plane.h"      
Plane::Plane(int x, int y, const QPixmap& pixmap, int speed)
	: Actor(x, y, pixmap), speed(speed)
{}
Plane::Plane(int x, int y)
    : Actor(x, y)
{
	speed = MIN_SPEED;
    this->pixmap = ImageManager::getImage(PLANE);  // Assuming PLANE is defined in ImageManager
    this->length = pixmap.width();
}
void Plane::action(Stat stat)  {
    std::cout << "Obstacle -- plane action()" << std::endl;
}
int Plane::getSpeed() const {
    return speed; 
}
void Plane::setY(int y)
{
	this->posY = y;
}
