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
    bool Prochaine = false;
    Button* NextPage;
    int score = 0;
    int SkinChecked = 0;
    bool ButtonPushed();

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
    QCheckBox* NewPlayer;
    QCheckBox* Plane;
    QCheckBox* Chopper;
    QCheckBox* Jet;
    bool PlayerIsNew = false;

};

