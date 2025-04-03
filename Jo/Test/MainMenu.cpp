#include "MainMenu.h"

MainMenu::MainMenu()
{
	setWindowTitle("Authentification");
	resize(1920, 1080);

	Authen = new UserName("Nouvelle Page!!!", this);
	Authen->setGeometry(430, 255, 400, 35);
}
