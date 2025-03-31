#pragma once
#include <QCheckBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QMainWindow>
#include <iostream>
#include <QDebug> 
#include <QTimer>

#include "ui_UIGraphiqueTest.h"
#include "ui_PageAcceuil.h"

class myWindow : public QWidget
{
	Q_OBJECT
public:
	explicit myWindow(QWidget* parent = nullptr);
	//void customFunction();
private slots:
		//void onButtonClicked();
private:
	bool ok = true;
	QTimer* timer;
	Ui::Form*ui;
	void moveLabel();
};