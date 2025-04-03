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

class QDashboard : public QGraphicsPixmapItem {
private:
    QGraphicsTextItem* statsText;

    QPixmap tankPixmap;
    QGraphicsPixmapItem* tankPixmapItem;
    QGraphicsPixmapItem* backgroundItem;
    int lastFillHeight = 0;
    Stat* stat;
public:
    QDashboard(Stat* s);
    void update();
    void updateTexte();
    void updatePixmapTank();
private:
    
};
