#include "QDashboard.h"
#include <QFontDatabase>
#include <QGraphicsProxyWidget>
#include <QDebug>
QDashboard::QDashboard(Stat* s) {
	stat = s;
	statsText = new QGraphicsTextItem();
	statsText = gameScene->addText("");
	statsText->setZValue(1);

	QPixmap background("sprites/Stat/Dashboard.png");
	background = background.scaled(WIDTH_DASHBOARD, HEIGHT_DASHBOARD, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	backgroundItem = new QGraphicsPixmapItem();
	backgroundItem->setPixmap(background);
	backgroundItem->setZValue(0);
	backgroundItem->setPos(-10, -10);
	gameScene->addItem(backgroundItem);
	
	
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

	gameScene->addItem(statsText);

	gameScene->addItem(itemAiguilleFuel);
	gameScene->addItem(itemAiguilleHeight);
	gameScene->addItem(itemAiguilleSpeed);

	gameScene->addItem(itemCadranFuel);
	gameScene->addItem(itemCadranHeight);
	gameScene->addItem(itemCadranSpeed);
	
}
void QDashboard::update()
{
	updateTexte();
	updateCadran(QPointF(0 , 0));
}

void QDashboard::updateTexte() {
	QString textHtml = "<div style='"
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
		"</div>";
	statsText->setHtml(textHtml);
}

void QDashboard::updatePixmapTank()
{
	// Assurer que tankPixmap a une taille correcte
	if (tankPixmap.isNull()) {
		tankPixmap = QPixmap(200, 300);
	}
	tankPixmap.fill(Qt::transparent);

	QPainter painter(&tankPixmap);
	painter.setRenderHint(QPainter::Antialiasing);

	// Dessiner le contour du réservoir
	painter.setPen(Qt::black);
	painter.drawRect(10, 10, 100, 280);

	// Dessiner le gaz
	int fill_height = static_cast<int>(2.8 * stat->getFuel());
	painter.setBrush(QColor(0, 150, 255, 180));
	painter.drawRect(10, 290 - fill_height, 100, fill_height);

	// Mettre à jour l'image affichée
	QTransform transform;
	transform.scale(2, 1.5);
	tankPixmapItem->setTransform(transform);
	tankPixmapItem->setPixmap(tankPixmap);
}
void QDashboard::updateCadran(const QPointF& position)
{
	// Cadran
	itemCadranFuel->setZValue(2);
	itemCadranFuel->setPos(position);
	itemCadranHeight->setZValue(2);
	itemCadranHeight->setPos(position + QPointF(0, 400));
	itemCadranSpeed->setZValue(2);
	itemCadranSpeed->setPos(position + QPointF(0, 800));

	itemAiguilleFuel->setZValue(3);
	itemAiguilleHeight->setZValue(3);
	itemAiguilleSpeed->setZValue(3);


	// FUEL
	itemAiguilleFuel->setOffset(0, -itemAiguilleFuel->pixmap().height() / 2);
	itemAiguilleFuel->setTransformOriginPoint(0, itemAiguilleFuel->pixmap().height() / 2);
	QPointF centreCadran = position + QPointF(itemCadranFuel->pixmap().width() / 2.0, itemCadranFuel->pixmap().height() / 3);
	itemAiguilleFuel->setPos(centreCadran);
	itemAiguilleFuel->setRotation(stat->getFuel() - MAX_FUEL);

	// HAUTEUR
	if (stat->getHeight() != lastHeight)
	{
		lastHeight = stat->getHeight();
		itemAiguilleHeight->setOffset(0, -itemAiguilleHeight->pixmap().height() / 2);
		itemAiguilleHeight->setTransformOriginPoint(0, itemAiguilleHeight->pixmap().height() / 2);
		QPointF centreCadran = position + QPointF(itemCadranHeight->pixmap().width() / 2.0, itemCadranHeight->pixmap().height() / 3) + QPointF(0, 400);
		itemAiguilleHeight->setPos(centreCadran);
		itemAiguilleHeight->setRotation((lastHeight- 1080)/ 10);
	}
	
	// VITESSE
	if (stat->getSpeed() != lastSpeed)
	{
		lastSpeed = stat->getSpeed();
		itemAiguilleSpeed->setOffset(0, -itemAiguilleSpeed->pixmap().height() / 2);
		itemAiguilleSpeed->setTransformOriginPoint(0, itemAiguilleSpeed->pixmap().height() / 2);
		QPointF centreCadran = position + QPointF(itemCadranSpeed->pixmap().width() / 2.0, itemCadranSpeed->pixmap().height() / 3) + QPointF(0, 800);
		itemAiguilleSpeed->setPos(centreCadran);
		itemAiguilleSpeed->setRotation(stat->getSpeed() - MAX_SPEED);
	}
}
