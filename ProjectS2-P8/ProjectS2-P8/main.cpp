
#include <iostream>
#include <string>

#include <stdlib.h> 
#include <windows.h>
#include <chrono>
#include <thread>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"
#include "const.h"

#include "ConnectionSerie.h"
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
	ConnectionSerie connection;
    ///// ----- test connection serie ----- ///

 //   json msg_envoi;
 //   msg_envoi["BAR"] = 5;
 //   msg_envoi["led"] = 1;
	//msg_envoi["LCD"] = 1220;
	//int jeuBas = 0;
	//int jeuHaut = 0;
	//int pot = 0;
	//int bpG = 0;
	//int bpD = 0;
	//int bpH = 0;
	//int bpB = 0;
	//int axeX = 0;
 //   while (1)
 //   {
 //       if (connection.hasData())
 //       {
 //           jeuBas = connection.getValue("JB");
 //           jeuHaut = connection.getValue("JH");
 //           pot = connection.getValue("pot");
 //           bpG = connection.getValue("BG");
 //           bpD = connection.getValue("BD");
 //           bpH = connection.getValue("BH");
 //           bpB = connection.getValue("BB");
 //           axeX = connection.getValue("AX");
 //           cout << "bouton gauche : " << bpG << endl;
 //           cout << "bouton droit : " << bpD << endl;
 //           cout << "bouton haut : " << bpH << endl;
 //           cout << "bouton bas : " << bpB << endl;
 //           cout << "axe X : " << axeX << endl;
 //           cout << "pot : " << pot << endl;
 //           cout << "jeuHaut : " << jeuHaut << endl;
 //           cout << "jeuBas : " << jeuBas << endl;
 //           cout << "--------------------------------" << endl;
 //       }
 //       connection.Envoie(msg_envoi);
 //       //cout << "envoi : " << msg_envoi.dump() << endl;
 //       Sleep(80);
 //   }



    //EcranPrincipal ecran;
    //ecran.printMenu();
    //Player* player = new Player(ecran.getUserName(), ecran.getScore());
    system("cls");
    Game* game = new Game();
    while (!game->takeoff()) {};
    while (!game->stat->close)
    {
        sleep_for(std::chrono::milliseconds(100 / (game->stat->getSpeed() / 2)));
        system("cls");
        game->update();
    }
    system("cls");
    if (game->stat->landing)
    {
        game->touchDown();
    }
    return 0;

 }



