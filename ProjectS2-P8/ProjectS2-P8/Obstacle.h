#pragma once
#include <iostream>
#include "Actor.h"
#include "const.h"
#include "ImageManager.h"

//#define gazFuel 1
//#define windFuel -1
//#define birdFuel -2
//#define maxFuel 100

using namespace std;

class Gaz : public Actor {
public:
    Gaz(int x, int y, const QPixmap& pixmap, int speed)
        : Actor(x, y, pixmap), speed(speed) {
    }
    Gaz(int x, int y) : Actor(x, y) {
        speed = OBSTACLE_SPEED;

        this->pixmap = ImageManager::getImage(GAZ);

        this->length = pixmap.width();
    }
    void action(Stat stat) override {
        stat.changeFuel(GAZ_FUEL);
		
        // std::cout << "Obstacle -- Gaz action()" << std::endl;
    }
    int getSpeed() const override { return speed; }

private:
    int speed;
};

class Wind : public Actor {
public:
    Wind(int x, int y, const QPixmap& pixmap, int speed)
        : Actor(x, y, pixmap), speed(speed) {
    }

    Wind(int x, int y) : Actor(x, y) {
        speed = OBSTACLE_SPEED;
        this->pixmap = ImageManager::getImage(WIND);
        this->length = pixmap.width();
    }

    void action(Stat stat) override {
        stat.changeFuel(WIND_FUEL);
    }

    int getSpeed() const override {
        return speed;
    }

private:
    int speed;
};


class Tree : public Actor {
public:
    Tree(int x, int y, const QPixmap& pixmap, int speed)
        : Actor(x, y, pixmap), speed(speed) {
    }

    Tree(int x, int y) : Actor(x, y) {
        speed = OBSTACLE_SPEED;
        this->pixmap = ImageManager::getImage(TREE);
        this->length = pixmap.width();
    }

    void action(Stat stat) override {
        stat.close = true;
    }

    int getSpeed() const override {
        return speed;
    }

private:
    int speed;
};


class Bird : public Actor {
public:
    Bird(int x, int y, const QPixmap& pixmap, int speed)
        : Actor(x, y, pixmap), speed(speed) {
    }

    Bird(int x, int y) : Actor(x, y) {
        speed = OBSTACLE_SPEED;
        this->pixmap = ImageManager::getImage(BIRD);
        this->length = pixmap.width();
    }

    void action(Stat stat) override {
        stat.changeFuel(BIRD_FUEL);
    }

    int getSpeed() const override {
        return speed;
    }

private:
    int speed;
};

