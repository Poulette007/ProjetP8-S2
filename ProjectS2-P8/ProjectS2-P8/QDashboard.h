#pragma once
#include <QApplication>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QVBoxLayout>
#include <QSlider>
#include <QPainter>
#include <QColor>
#include "Stat.h"
#include "const.h"
#include "ImageManager.h"

class QDashboard : public QGraphicsPixmapItem {
private:
    QGraphicsTextItem* statsText;

    QPixmap tankPixmap;
    QGraphicsPixmapItem* tankPixmapItem;
    QGraphicsPixmapItem* backgroundItem;


	QGraphicsPixmapItem* itemAiguilleFuel;
	QGraphicsPixmapItem* itemAiguilleHeight;
	QGraphicsPixmapItem* itemAiguilleSpeed;

	QGraphicsPixmapItem* itemCadranFuel;
	QGraphicsPixmapItem* itemCadranHeight;
	QGraphicsPixmapItem* itemCadranSpeed;
    
    
	int lastHeight = 0;
	int lastSpeed = 0;
    int lastFillHeight = 0;
    Stat* stat;
public:
    QDashboard(Stat* s);
    void update();
    void updateTexte();
	void updateCadran(const QPointF& position);
    void updatePixmapTank();
private:
    
};
