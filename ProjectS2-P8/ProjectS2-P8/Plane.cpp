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

