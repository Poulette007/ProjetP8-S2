#include "GameOver.h"
#include <QFile>
#include <QMessageBox>
GameOver::GameOver()
{
	setWindowTitle("Game Over");
	resize(1920, 1080);

	//Bouton recommencer le jeu
	NextPage = new Button(this);
	NextPage->setText("Recommencer!");
	NextPage->setGeometry(1360, 865, 150, 50);
}

void GameOver::paintEvent(QPaintEvent* event)
{
	QPainter painter(this);
	//Image de fond
	QPixmap fond("sprites/background/GameOver.png");
	painter.drawPixmap(0, 0, width(), height(), fond);

}
