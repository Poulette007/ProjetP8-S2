#include "Actor.h"

Actor::Actor(int x, int y, int length, std::string sprite)
{
    setPosition(x, y, length);
}
Actor::Actor(int x, int y)
{
    setPosition(x, y, getSprite().length());
}
void Actor::setX(int x) {
    this->posX = x;
}

int Actor::getX() {
    return posX;
}

int Actor::getY() {
    return posY;
}

void Actor::afficher()
{
    std::cout << getSprite();
}
int Actor::getLength() {
    return length;
}

std::string Actor::getSprite() {
    return sprite;
}

void Actor::setPosition(int x, int y, int length) {
    setX(x);
    this->posY = y;
    this->length = length;
}