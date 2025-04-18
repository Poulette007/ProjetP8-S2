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
#include <QRandomGenerator>

#include <QTimer>
#include <QRandomGenerator>


extern QGraphicsScene* gameScene;
extern QGraphicsPixmapItem* BackGroundVol;

#define GAZ_FUEL 15
#define WIND_FUEL -10
#define BIRD_FUEL -20

#define MAX_SPEED 100
#define MIN_SPEED 2
#define MAX_FUEL 100
#define MIN_FUEL 0

#define ACTOR_POS_X 100
#define START_PLANE_X 5

#define OBSTACLE_SPEED 2

#define SPACEBAR 200
#define CLAVIER_W 201
#define CLAVIER_A 202
#define CLAVIER_S 203
#define CLAVIER_D 204
#define BOUTON_BAS 205
#define BOUTON_GAUCHE 206
#define BOUTON_DROIT 207
#define BOUTON_HAUT 208
#define JOY_HAUT 209
#define JOY_BAS 210
#define JOY_GAUCHE 211
#define JOY_DROIT 212
#define SHAKE 213

#define OBSTACLE_MIN_DIST 700   
#define SCREEN_HEIGHT 1080
#define RANGEE_HEIGHT (SCREEN_HEIGHT / 4) 


// new defines
#define WIDTH_DASHBOARD 1920 / 5 + 10
#define HEIGHT_DASHBOARD 1080 + 10