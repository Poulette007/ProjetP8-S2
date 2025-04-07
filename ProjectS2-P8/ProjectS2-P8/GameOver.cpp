#include "GameOver.h"

GameOver::GameOver(bool victoire)
{
	setWindowTitle("Game Over");
	estVictoire = victoire;
	Retour = new Button(this);
	Retour->setText("Retour au menu");
	Retour->setGeometry(1360, 865, 150, 50);
	resize(1920, 1080);

}

bool GameOver::ButtonPushed()
{
	return false;
}
void GameOver::setVictoire(bool victoire)
{
	estVictoire = victoire;
	update();	//refait le paint event
}

void GameOver::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	if (estVictoire) {
		//Image de fond
		QPixmap fond("sprites/background/youwin.png");
		painter.drawPixmap(rect(), fond);
		qDebug() << "Victoire";
	}
	else {
		//Image de fond
		QPixmap fond("sprites/background/gameover.png");
		painter.drawPixmap(rect(), fond);
		qDebug() << "Defaite";
	}
	QWidget::paintEvent(event);
}
