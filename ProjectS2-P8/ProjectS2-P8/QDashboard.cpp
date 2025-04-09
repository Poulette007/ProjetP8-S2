#include "QDashboard.h"
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QDebug>
#include "UserName.h"

QDashboard::QDashboard(Stat* s) {
	stat = s;
	statsText = new QGraphicsTextItem();
	statsText = gameScene->addText("");
	statsText->setZValue(4);
	lastHeight = 100;
	QPixmap background("sprites/Stat/Dashboard.png");
	background = background.scaled(WIDTH_DASHBOARD, HEIGHT_DASHBOARD, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	backgroundItem = new QGraphicsPixmapItem();
	backgroundItem->setPixmap(background);
	backgroundItem->setZValue(4);
	backgroundItem->setPos(-10, -10);
	gameScene->addItem(backgroundItem);
	
	QString gaz = "Gaz: " + QString::number(stat->getFuel());
	infoFuel = new FormatTextPixmap(gaz, this, 16, TEXTE, Qt::white);
	infoFuel->setZValue(4);
	QString height = "Hauteur: " + QString::number(stat->getHeightRevers());
	infoHeight = new FormatTextPixmap(height, this, 16, TEXTE, Qt::white);
	infoHeight->setZValue(4);

	QString speed = "Vitesse: " + QString::number(stat->getSpeed()) + "00km/h";
	infoSpeed = new FormatTextPixmap(speed, this, 16, TEXTE, Qt::white);
	infoSpeed->setZValue(4);

	QString score = "Score: " + QString::number(stat->getScore());
	infoScore = new FormatTextPixmap(score, this, 16, TEXTE, Qt::white);
	infoScore->setZValue(4);


	itemAiguilleFuel = new QGraphicsPixmapItem();
	itemAiguilleHeight = new QGraphicsPixmapItem();
	itemAiguilleSpeed = new QGraphicsPixmapItem();

	itemCadranFuel = new QGraphicsPixmapItem();
	itemCadranHeight = new QGraphicsPixmapItem();
	itemCadranSpeed = new QGraphicsPixmapItem();
	
	itemAiguilleFuel->setPixmap(ImageManager::getInstance().getImage(AIGUILLE).scaled(
			100, 100,
			Qt::KeepAspectRatio,
			Qt::SmoothTransformation
		));
	itemAiguilleHeight->setPixmap(ImageManager::getInstance().getImage(AIGUILLE).scaled(
		100, 100,
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	));
	itemAiguilleSpeed->setPixmap(ImageManager::getInstance().getImage(AIGUILLE).scaled(
		100, 100,
		Qt::KeepAspectRatio,
		Qt::SmoothTransformation
	));

	itemCadranFuel->setPixmap(ImageManager::getInstance().getImage(CADRAN));
	itemCadranHeight->setPixmap(ImageManager::getInstance().getImage(CADRAN));
	itemCadranSpeed->setPixmap(ImageManager::getInstance().getImage(CADRAN));


	update();
	// Trust place le label
	statsText->setPos(backgroundItem->boundingRect().width() / 2 - statsText->boundingRect().width() / 2,
		backgroundItem->boundingRect().height() / 2 - statsText->boundingRect().height() / 2);

	//gameScene->addItem(statsText);

	gameScene->addItem(itemAiguilleFuel);
	gameScene->addItem(itemAiguilleHeight);
	gameScene->addItem(itemAiguilleSpeed);

	gameScene->addItem(itemCadranFuel);
	gameScene->addItem(itemCadranHeight);
	gameScene->addItem(itemCadranSpeed);

	gameScene->addItem(infoFuel);
	gameScene->addItem(infoHeight);
	gameScene->addItem(infoSpeed);
	gameScene->addItem(infoScore);
}
void QDashboard::update()
{
	updateTexte();
	updateCadran(QPointF(50 , 100));
}

void QDashboard::updateTexte() {
	/*QString textHtml = "<div style='"
		"color: #39200d;"
		"font-size: 50px;"
		"font-weight: bold;"
		"font-family: \"Press Start 2P\", Courier, monospace;"
		"text-shadow: 3px 3px 0px black, -3px -3px 0px black, 3px -3px 0px black, -3px 3px 0px black, "
		"2px 2px 0px black, -2px -2px 0px black, 2px -2px 0px black, -2px 2px 0px black;"
		"'>"
		"Gaz: " + QString::number(stat->getFuel()) + "<br><br>"
		"Hauteur: " + QString::number(stat->getHeight()) + "<br><br>"
		"Vitesse: " + QString::number(stat->getSpeed()) + "<br><br>"
		"Score: " + QString::number(stat->getScore()) +
		"</div>";*/
	// statsText->setHtml(textHtml);
}
void QDashboard::updateCadran(const QPointF& position)
{
	// Cadran
	itemCadranFuel->setZValue(4);
	itemCadranFuel->setPos(position);
	itemCadranHeight->setZValue(4);
	itemCadranHeight->setPos(position + QPointF(0, 300));
	itemCadranSpeed->setZValue(4);
	itemCadranSpeed->setPos(position + QPointF(0, 600));

	itemAiguilleFuel->setZValue(5);
	itemAiguilleHeight->setZValue(5);
	itemAiguilleSpeed->setZValue(5);


	// FUEL
	itemAiguilleFuel->setOffset(0, -itemAiguilleFuel->pixmap().height() / 2);
	itemAiguilleFuel->setTransformOriginPoint(0, itemAiguilleFuel->pixmap().height() / 2);
	QPointF centreCadran = position + QPointF(itemCadranFuel->pixmap().width() / 2.0, itemCadranFuel->pixmap().height() / 3);
	itemAiguilleFuel->setPos(centreCadran);
	infoFuel->setPos(position.x(), position.y() + itemCadranFuel->pixmap().height()/2 + 50);
	itemAiguilleFuel->setRotation(stat->getFuel() - MAX_FUEL);
	infoFuel->setText("Gaz: " + QString::number(stat->getFuel()) + "k litre");

	// HAUTEUR
	if (stat->getHeightRevers() != lastHeight)
	{
		lastHeight = stat->getHeightRevers();
		itemAiguilleHeight->setOffset(0, -itemAiguilleHeight->pixmap().height() / 2);
		itemAiguilleHeight->setTransformOriginPoint(0, itemAiguilleHeight->pixmap().height() / 2);
		QPointF centreCadran = position + QPointF(itemCadranHeight->pixmap().width() / 2.0, itemCadranHeight->pixmap().height() / 3) + QPointF(0, 300);
		itemAiguilleHeight->setPos(centreCadran);
		infoHeight->setPos(position.x(), position.y() + itemCadranHeight->pixmap().height() + 200);
		itemAiguilleHeight->setRotation((lastHeight - 1080)/ 10);
		infoHeight->setText("Hauteur: " + QString::number(stat->getHeightRevers()) + "m");
	}
	
	// VITESSE
	if (stat->getSpeed() != lastSpeed)
	{
		lastSpeed = stat->getSpeed();
		itemAiguilleSpeed->setOffset(0, -itemAiguilleSpeed->pixmap().height() / 2);
		itemAiguilleSpeed->setTransformOriginPoint(0, itemAiguilleSpeed->pixmap().height() / 2);
		QPointF centreCadran = position + QPointF(itemCadranSpeed->pixmap().width() / 2.0, itemCadranSpeed->pixmap().height() / 3) + QPointF(0, 600);
		itemAiguilleSpeed->setPos(centreCadran);
		infoSpeed->setPos(position.x(), position.y() + itemCadranSpeed->pixmap().height() + 500);
		itemAiguilleSpeed->setRotation(stat->getSpeed() - MAX_SPEED);
		infoSpeed->setText("Vitesse: " + QString::number(stat->getSpeed()) + "00km/h");
	}
	infoScore->setText("Score: " + QString::number(stat->getScore()));
	infoScore->setPos(1920 / 5 - 300, 50);

}