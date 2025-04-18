#pragma once
#include <QApplication>
#include <Qwidget>
#include <Button.h>
#include <UserName.h>
#include <SmallText.h>
#include <QCheckBox>
#include <QlineEdit>
#include <MainMenu.h>
#include "Stat.h"

class login :
    public QWidget
{
public:
    login();
    Button* NextPage;
    int score = 0;
    static int SkinChecked;
    bool ButtonPushed();
    QCheckBox* Plane;
    QCheckBox* Chopper;
    QCheckBox* Jet;
    QCheckBox* NewPlayer;
    bool PlayerIsNew = false;

private:
    void PlaneCheckBox();
    void ChopperCheckBox();
    void JetCheckBox();
    void NewPlayerCheckBox();
    bool userExists(QString);
    void addUser(QString userName);
    void paintEvent(QPaintEvent* event) override;
    QMap<int, QString> getBestScore(int nombre);
    UserName* Authen;
    QLineEdit* Nom;
    UserName* New;
    

};

