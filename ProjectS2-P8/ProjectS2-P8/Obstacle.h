#pragma once
#include <iostream>

#include "Actor.h"

using namespace std;

class Gaz : public Actor {
public:
    Gaz(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    void action() override {
        std::cout << "Obstacle -- Gaz action()" << std::endl;
    }
    int getSpeed() const override { return speed; }

private:
    int speed;
};

class Wind : public Actor {
public:
    Wind(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    void action() override {
        std::cout << "Obstacle -- wind action()" << std::endl;
    }
    int getSpeed() const override { return speed; }


private:
    int speed;
};

class Tree : public Actor {
public:
    Tree(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    void action() override {
        std::cout << "Obstacle -- Tree action()" << std::endl;
    }
    int getSpeed() const override { return speed; }

private:
    int speed;
};

class Bird : public Actor {
public:
    Bird(int x, int y, int length, std::string sprite, int speed)
        : Actor(x, y, length, sprite), speed(speed) {
    }
    void action() override {
        std::cout << "Obstacle -- Bird action()" << std::endl;
    }
    int getSpeed() const override { return speed; }

private:
    int speed;
};
