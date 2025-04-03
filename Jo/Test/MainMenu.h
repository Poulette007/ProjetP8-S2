#pragma once
#include <login.h>

class MainMenu :
    public QWidget
{
public:
    MainMenu();

private:
    UserName* Authen;
};

