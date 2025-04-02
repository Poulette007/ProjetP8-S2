#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsDropShadowEffect>
#include <QLabel>
#include <QMovie>
#include <QColor>
#include <QTimer>

extern QGraphicsScene* gameScene;

#define GAZ_FUEL 30
#define WIND_FUEL -20
#define BIRD_FUEL -40

#define MAX_SPEED 100
#define MIN_SPEED 2
#define MAX_FUEL 1000
#define MIN_FUEL 0

#define ACTOR_POS_X 100
#define START_PLANE_X 5

#define OBSTACLE_SPEED 2
