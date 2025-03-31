#include <qapplication.h>
#include "Window.h"

int main(int argc, char* argv[])
{
	QApplication app(argc, argv);
	myWindow window;
	
	window.show();
	return app.exec();
}