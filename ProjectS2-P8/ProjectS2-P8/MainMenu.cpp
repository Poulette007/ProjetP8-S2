#include "MainMenu.h"

MainMenu::MainMenu()
{
	setWindowTitle("Menu Principale");
	resize(1920, 1080);

	TextScore = new UserName("Tableau des meilleurs scores", this);
	TextScore->setGeometry(430, 125, 650, 35);

	//Bouton commencer le jeu
	NextPage = new Button(this);
	NextPage->setText("Commencer!");
	NextPage->setGeometry(1360, 865, 150, 50);

	//Bouton pour passer a la page suivante
	BackPage = new Button(this);
	BackPage->setText("Revenir");
	BackPage->setGeometry(5, 865, 150, 50);
}

void MainMenu::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //Image de fond
    QPixmap fond("sprites/background/Sky.png");
    painter.drawPixmap(0, 0, width(), height(), fond);

	//Image Tour de control
	QPixmap tower("sprites/background/ControlTower.png");
	painter.drawPixmap(0, 175, 175, 725, tower);

	//Image aeroport
	QPixmap airport("sprites/background/Airport.png");
	painter.drawPixmap(0, 650, 624, 242, airport);

	//Image de la piste
	QPixmap runway("sprites/background/PisteComplete.png");
	painter.drawPixmap(0, 850, runway);

	//Image avion
	QPixmap avion("sprites/avion/avion1.png");
	painter.drawPixmap(150, 795, 200, 150, avion);

	//Image du tableau des scores
    QPixmap bord("sprites/background/LeaderbordTableau.png");
    painter.drawPixmap(505, 295, 485, 485, bord);
	QPixmap crown("sprites/background/LeaderbordCrown.png");
	painter.drawPixmap(505, 100, 485, 195, crown);


    //On fini le paint event
    QWidget::paintEvent(event);
}