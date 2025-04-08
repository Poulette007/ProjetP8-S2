#pragma once
#include <string>
#include <iostream>
#include <QGraphicsPixmapItem>
#include <qpixmap.h>
#include "Stat.h"
class Actor : public QGraphicsPixmapItem {
public:
	// Avec QPixmap
	Actor(int x, int y);

	virtual void action(Stat stat) = 0;
	virtual int getSpeed() const = 0;

	QPixmap getPixmap() const;
	// debug
	void afficher() const;


};
