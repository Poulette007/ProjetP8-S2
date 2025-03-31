#pragma once
#include <QMainWindow>
#include <QWidget>
#include <QGraphicsScene>
#include <qgraphicsview.h>
#include <QGraphicsProxyWidget>
#include <QVBoxLayout>
#include <QTimer>
#include <QMap>
#include <QSet>

#include "Game.h"
#include "ImageManager.h"

class UI_Game : public QMainWindow
{
	Q_OBJECT
public:
	UI_Game(Game &game);
	void update();
	void readKeyboard();

private:
	void updateObstacles();
	QGraphicsScene* scene;
	QGraphicsView* view;

	Game* game;
	QTimer* timer;
	QTimer* keyboardTimer;

	QGraphicsProxyWidget* proxyPlane;
	QMap<Actor*, QGraphicsProxyWidget*> proxyMap;


};

