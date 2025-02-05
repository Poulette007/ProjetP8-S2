#include "Plane.h"
Plane::Plane(int x, int y, int length, std::string sprite, int speed)
    : Actor(x, y, length, sprite), speed(speed) {
}
void Plane::action()  {
    std::cout << "Obstacle -- plane action()" << std::endl;
}
int Plane::getSpeed() const {
    return speed; 
}

