#include "Actor.h"


Actor::Actor(int x, int y, const QPixmap& pixmap)
 : posX(x), posY(y), length(pixmap.width()), pixmap(pixmap)
{
}

Actor::Actor(int x, int y)
	: posX(x), posY(y), length(0)
{}
void Actor::setX(int x) {
    this->posX = x;
}

int Actor::getX() {
    return posX;
}

int Actor::getY() {
    return posY;
}

int Actor::getLength() {
    return length;
}

std::string Actor::getSprite() {
    return sprite;
}
QPixmap Actor::getPixmap() const {
    return this->pixmap;
}
void Actor::setPosition(int x, int y, int length) {
    setX(x);
    this->posY = y;
    this->length = length;
}
void Actor::afficher() const {
    std::cout << "DEBUG -- Actor à position (" << posX << ", " << posY << ") avec une longueur de " << length << std::endl;
}