#pragma once
#include <login.h>

class MainMenu :
    public QWidget
{
public:
    MainMenu();
    Button* NextPage;
    Button* BackPage;

private:
    UserName* TextScore;
};
