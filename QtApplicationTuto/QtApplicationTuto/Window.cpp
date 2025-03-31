#include "Window.h"

myWindow::myWindow(QWidget * parent) : QWidget(parent), ui(new Ui::Form)
{
	ui->setupUi(this);
	
	//connect(ui.pushButton, &QPushButton::clicked, this, &myWindow::onButtonClicked);
	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &myWindow::moveLabel);
	timer->start(500);
}

void myWindow::moveLabel()
{/*
	if (ok)
	{
		ui->ZoneDeText->setText("allo");
		ok = false;
	}
	else
	{
		ok = true;
		ui->ZoneDeText->setText("test");
	}
	QPoint position = ui->ZoneDeText->pos();
	position.setY(position.y() + 5);
	if (position.y() > 300) { // Si le label atteint la limite inférieure de la fenêtre
		position.setY(10); // Retourner au point de départ
	}
	ui->ZoneDeText->move(position);
*/
}
