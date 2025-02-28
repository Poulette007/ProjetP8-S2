#pragma once
#include <iostream>
#include "Actor.h"
#include "const.h"

//#define gazFuel 1
//#define windFuel -1
//#define birdFuel -2
//#define maxFuel 100

using namespace std;

class Gaz : public Actor {
public:
    Gaz(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    Gaz(int x, int y) : Actor(x, y)
    {
        speed = OBSTACLE_SPEED;
        setSprite("Gaz");
    }
    void action(Stat stat) override {
        stat.changeFuel(GAZ_FUEL);
        // std::cout << "Obstacle -- Gaz action()" << std::endl;
    }
    int getSpeed() const override { return speed; }
    void setSprite(std::string sprite) override {
        this->sprite = sprite;
    }

private:
    int speed;
};

class Wind : public Actor {
public:
    Wind(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    Wind(int x, int y) : Actor(x, y)
    {
        setSprite("Vent");
		speed = OBSTACLE_SPEED;
    }
    void action(Stat stat) override {
		stat.changeFuel(WIND_FUEL);
        // std::cout << "Obstacle -- wind action()" << std::endl;
    }
    int getSpeed() const override { return speed; }
    void setSprite(std::string sprite) override {
        this->sprite = sprite;
    }

private:
    int speed;
};

class Tree : public Actor {
public:
    Tree(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    Tree(int x, int y) : Actor(x, y)
    {
        speed = OBSTACLE_SPEED;
        setSprite("Arbre");
    }
    void action(Stat stat) override {
        stat.close = true;
        // std::cout << "Obstacle -- Tree action()" << std::endl;
    }
    int getSpeed() const override { return speed; }
    void setSprite(std::string sprite) override {
        this->sprite = sprite;
    }
private:
    int speed;
};

class Bird : public Actor {
public:
    Bird(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    Bird(int x, int y) : Actor(x, y)
    {
        speed = OBSTACLE_SPEED;
        setSprite("Oiseau");
    }
    void action(Stat stat) override {
		stat.changeFuel(BIRD_FUEL);
        // std::cout << "Obstacle -- Bird action()" << std::endl;
    }
    int getSpeed() const override { return speed; }
    void setSprite(std::string sprite) override {
        this->sprite = sprite;
    }
private:
    int speed;
};
