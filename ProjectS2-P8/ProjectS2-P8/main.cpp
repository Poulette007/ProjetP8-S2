/*
    CPP avec Qt
    TODO : (fichier, fonction)
    1 ->
    Actor, sprite, changer pour QPixmap
    Object, sprite, changer avec heritage
    Plane, sprite changer avec heritage
    Actor et Object : voir pour la taille, comment faire.

    2 ->
    Game et UI_Game
    Mettre les print des object
    Changer fonction GotoXY pour qt. + voir si bouger label est mieux
*/
#include <iostream>
#include <stdlib.h> 
#include <windows.h>
#include <chrono>
#include <thread>
#include <QApplication>
#include <QTimer>

#include "Obstacle.h"
#include "EcranPrincipal.h"
#include "Plane.h"
#include "Game.h"
#include "Player.h"
#include "const.h"
#include "UI_Game.h"

using namespace std;

void gotoxy(int x, int y)
{
	COORD coord = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), (coord));
}



int main(int argc, char* argv[]) {
    QApplication app(argc, argv);



    ImageManager imageManager;


	Game* game = new Game();
    UI_Game* uiGame = new UI_Game(*game);
    uiGame->show();
    

    QTimer timer;
    QObject::connect(&timer, &QTimer::timeout, [&]() {
        game->update();
        uiGame->update();
		});
    timer.start(16);
    

    return app.exec();
}
void printArt() {
    std::string art = R"(
                   .:..   .*+                                                     .##-%=.         +@
                   .%-    .*+                                                  .#@#:...=%%=       +@
                   .%-    .*+                                                   .:*%-.*@=:..      +@
                   .%-    .*+                                               ....   .%@*. ...      +@
      .-++++++++++++@*+++++%#+++++++++++=:.         ..-+#%=..          ..-*%%%@@%-. -*.           +@
      *#................................:@-        =@#=...-==+.   ..-*%#=.:@*:..*#....           +@
      **................................:%-   ...  :@+.     .:#@*-#@#-.    :%*::*%%%%%@%-.        +@
      .=#@############################%%*:.:*@@%%:...##.    .-#@#:...       ..::.......+@#.       +@
        :%:                          .+*  =%:...:##...+%:=#@*-...              .  .:*%#-+#.       +@
        .%+::::::::::::::::::::::::::-%+  -@-   ..-@+=#%*:..                 ..=#%#=:..=%-        +@
         .:-*%--------------------=@+-:.  .*#.     .-:..      ....:-==:.:=*#%#+-....:*%+..        +@
             *#...               :@=       .#=.           .+#%%%#*+=-=@*+-......:+#%+..           +@
             .*#..              :%=.        -@=.....::-=*#%@:      .-@-  ..:=#%%+:.               +@
              .*%.             :%+.          :@%###*=:.   :%.      :%*+%@%#=. ....                +@
               .*%#############@=             :%#:.     ..=#.     .%*:.                           +@
                -%............:%:              ..=%@@@@@%*#*.    .*#.                             +@
                -%.           :%:                .  ..   .**.   .+#..                             +@
                -%.           :%:                        .#+   .=@:.                              +@
                -%.           :%:                        .#*+#%%*:.             ....::..          +@
                -%.           :%:                         ... ...  .....::-=+#@@%##*=-##.         +@
                -%.           :%:                       .....:-=+*%@@%#*+-:.        ..=@:         +@
                -%.           :%:            ......:=#@@@@@%+-...       ......:-*%@@@@*..         +@
        .-=-..  -%.           :%:    ..:-+#%@@%#*+-:......      ..:-=*%%@@%#+-:.....=%.           +@
       +%-:-%*. -%.           :@%%@%*+=-:....  ........-+*#%%@%#+=-::-#..  .......  =%.           +@
      .%=   :%: -%.           :%:.      ...:-+*#%%%%*+-##....        -#.            =%..+:        +@
      .=%=-=%+. -%.           :%==+*#%%%#@*=:....      **.           -#.            =%..*-.       +@
       ..:=-... -%.           :%=:..... .%-            **.           -#.            =%..+:        +@
             +# -%.           :%#########@%############%%############%@#############%%..#=        +@
               .=@%%%%%%%%%%%%%%:.      .%-            **.           -#.            =%.           +@
               -@-.............=%:.     .%-            **.           -#.            =%.           +@
         ..=*##@=.....         .+%.     .%-            **.           -#.   ..-*##+-.=%.           +@
        .*%-.  .*@@%%%-.         =@:    .%-            **.           -#.  .+@=. ..+@@%%%=.       .%@
*.    ..*#.......-@:..#+..........=%.....%-............**............-#...+#........%:..##..    .#@@
    )";

    std::cout << art << std::endl;
}
