#include "QDashboard.h"
#include <QFontDatabase>
#include <QGraphicsProxyWidget>

QDashboard::QDashboard(Stat* s) {
	stat = s;
	statsText = gameScene->addText("");
	statsText->setZValue(1);

	QPixmap background("sprites/Stat/Dashboard.png");
	background = background.scaled(1920 / 3 + 10, 1080 + 50, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);

	backgroundItem = new QGraphicsPixmapItem();
	backgroundItem->setPixmap(background);
	backgroundItem->setPos(-10, -10);
	gameScene->addItem(backgroundItem);
	
	update();
	// Trust place le label
	statsText->setPos(backgroundItem->boundingRect().width() / 2 - statsText->boundingRect().width() / 2,
		backgroundItem->boundingRect().height() / 2 - statsText->boundingRect().height() / 2);

	gameScene->addItem(statsText);
}

void QDashboard::update()
{
	updateTexte();
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
