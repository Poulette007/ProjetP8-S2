#include "Actor.h"
Actor::Actor(int x, int y) {
    setPos(x, y);
}

QPixmap Actor::getPixmap() const {
    return pixmap();
}
void Actor::afficher() const {
    std::cout << "DEBUG -- Actor à position (" << x() << ", " << y() << ")" << std::endl;
}