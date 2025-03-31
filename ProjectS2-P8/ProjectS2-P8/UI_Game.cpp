#include "UI_Game.h"
#include <QLabel>

UI_Game::UI_Game(Game& game) :
	QMainWindow(),
	game(&game)
{
	// Variable initialization
	this->scene = new QGraphicsScene(this);
	this->view = new QGraphicsView(scene, this);

	view->setSceneRect(0, 0, 1000, 500);
	view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
	view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);


	QLabel* plane = new QLabel();
	QPixmap pixmap = this->game->plane->getPixmap();
	plane->setPixmap(pixmap);
	if (plane->pixmap().isNull()) {
		qDebug() << "Erreur de chargement de l'image de tettete l'avion";
	}
	plane->setFixedSize(pixmap.size());
	proxyPlane = scene->addWidget(plane);
	proxyPlane->setPos(0, 0); // Positionner le QLabel dans la scène
	//view->fitInView(proxyPlane, Qt::KeepAspectRatio);

	timer = new QTimer(this);
	connect(timer, &QTimer::timeout, this, &UI_Game::update);  // Connecte le signal timeout de QTimer à la méthode update de UI_Game
	timer->start(64);
	keyboardTimer = new QTimer(this);
	connect(keyboardTimer, &QTimer::timeout, this, &UI_Game::readKeyboard);
	keyboardTimer->start(100);  // Déclenche readKeyboard toutes les 10 ms

	this->setWindowTitle("Simulateur de vol");
	this->resize(1000, 500);
	view->setFixedSize(1000, 500);
	this->setCentralWidget(view);
	this->show();
	std::cout << "UI_Game - Constructeur";


	
}

void UI_Game::update()
{
	this->proxyPlane->setPos(this->game->plane->getX(), this->game->plane->getY() * 100);
	updateObstacles();
}

void UI_Game::updateObstacles()
{
	QSet<Actor*> currentActors;
	for (auto actor : this->game->listActor) {
		currentActors.insert(actor);
	}

	// Supprime les proxies pour les acteurs qui ne sont plus actifs
	auto it = proxyMap.begin();
	while (it != proxyMap.end()) {
		if (!currentActors.contains(it.key())) {
			QGraphicsProxyWidget* proxy = it.value();
			if (proxy) {
				scene->removeItem(proxy);  // Assure-toi que l'objet est retiré de la scène avant de le supprimer
				delete proxy->widget();  // Supprime le widget si proxy n'est pas nul
				//delete proxy;
			}
			it = proxyMap.erase(it);  // Efface l'entrée de la map et obtient le nouvel itérateur valide
		}
		else {
			++it;  // Avance seulement si aucun élément n'a été supprimé
		}
	}


	// Ajoute des proxies pour les nouveaux acteurs
	for (auto actor : this->game->listActor) {
		if (!proxyMap.contains(actor)) {
			QLabel* obstacle = new QLabel();
			QPixmap pixmap = actor->getPixmap();
			obstacle->setPixmap(pixmap);
			obstacle->setFixedSize(pixmap.size());

			QGraphicsProxyWidget* proxyObstacle = scene->addWidget(obstacle);
			proxyObstacle->setPos(actor->getX(), actor->getY() * 100);
			proxyMap.insert(actor, proxyObstacle);
		}
		
	}

}

void UI_Game::readKeyboard()
{
	if (GetAsyncKeyState('W') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		game->stat->readKeybord('W');
	}
	if (GetAsyncKeyState('S') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		game->stat->readKeybord('S');
	}
	if (GetAsyncKeyState('A') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		game->stat->readKeybord('A');
	}
	if (GetAsyncKeyState('D') < 0)   //on verifie si la fleche gauche ou D est pressee
	{
		game->stat->readKeybord('D');
	}
}
