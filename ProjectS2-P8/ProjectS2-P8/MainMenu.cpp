#include "MainMenu.h"

MainMenu::MainMenu()
{
	setWindowTitle("Menu Principale");
	resize(1920, 1080);

	TextScore = new UserName("Tableau des meilleurs scores", this);
	TextScore->setGeometry(420, 125, 650, 35);

	//Bouton commencer le jeu
	NextPage = new Button(this);
	NextPage->setText("Commencer!");
	NextPage->setGeometry(1300, 800, 150, 50);

	//Bouton pour passer a la page suivante
	BackPage = new Button(this);
	BackPage->setText("Revenir");
	BackPage->setGeometry(55, 800, 150, 50);
}
