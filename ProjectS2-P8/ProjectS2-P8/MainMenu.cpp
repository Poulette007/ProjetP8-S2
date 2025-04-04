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
	NextPage->setGeometry(1300, 800, 150, 50);

	//Bouton pour passer a la page suivante
	BackPage = new Button(this);
	BackPage->setText("Revenir");
	BackPage->setGeometry(55, 800, 150, 50);
}

void MainMenu::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //Image de fond
    QPixmap fond("sprites/background/LoginPage.png");
    painter.drawPixmap(0, 0, width(), height(), fond);

    //Image du tableau des scores
    QPixmap bord("sprites/background/LeaderbordTableau.png");
    painter.drawPixmap(505, 295, 485, 485, bord);
	QPixmap crown("sprites/background/LeaderbordCrown.png");
	painter.drawPixmap(505, 100, 485, 195, crown);

	//Image Tour de control
	QPixmap tower("sprites/background/ControlTower.png");
	painter.drawPixmap(0, 175, 175, 725, tower);
    //On fini le paint event
    QWidget::paintEvent(event);
}