#include "Plane.h"      
Plane::Plane(int x, int y)
    : Actor(x, y)
{
    speed = MIN_SPEED;
    //setPixmap(ImageManager::getImage(PLANE));
	setPixmap(ImageManager::getInstance().getImage(PLANE));
}
void Plane::action(Stat stat) {
    std::cout << "Obstacle -- plane action()" << std::endl;
}
int Plane::getSpeed() const {
    return speed;
}

void Plane::changePixmap(int skin)
{
		switch (skin)
		{
		case 0: // Plane
			setPixmap(ImageManager::getInstance().getImage(PLANE));
			break;
		case 1: // Chopper
			setPixmap(ImageManager::getInstance().getImage(JET));
			break;
		case 2: // Jet
			setPixmap(ImageManager::getInstance().getImage(CHOPPER));
			break;
		default:
			setPixmap(ImageManager::getInstance().getImage(PLANE));
			break;
		}
}

