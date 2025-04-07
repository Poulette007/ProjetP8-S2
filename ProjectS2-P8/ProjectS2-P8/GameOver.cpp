#include "GameOver.h"

GameOver::GameOver(bool victoire)
{
	setWindowTitle("Game Over");
	estVictoire = victoire;
	resize(1920, 1080);
	Retour = new Button(this);
	Retour->setText("Retour au menu");
	Retour->raise(); // force à être au-dessus visuellement
	Retour->show();
	Retour->setGeometry(650, 650, 150, 50);	
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
	painter.setBrush(QColor(255, 0, 0));  // rouge vif pour test
	if (estVictoire) {
		//Image de fond
		QPixmap fond("sprites/background/youwin.png");
		painter.drawPixmap(rect(), fond);
	}
	else {
		//Image de fond
		QPixmap fond("sprites/background/gameover.png");
		painter.drawPixmap(rect(), fond);
	}
	QWidget::paintEvent(event);
}
