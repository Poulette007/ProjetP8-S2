
#include <iostream>
#include <stdlib.h> 
#include <windows.h>
#include <chrono>
#include <thread>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"

using namespace std;

void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (coord));
}

int main()
{
	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;
	EcranPrincipal ecran;
	ecran.printMenu();
	Game *game = new Game();
	while (!game->isCollision)
	{
		sleep_for(0.5s);
		system("cls");
		game->update();
		
	}
	return 0;
}