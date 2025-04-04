#pragma once
#include <QApplication>
#include <Qwidget>
#include <Button.h>
#include <UserName.h>
#include <SmallText.h>
#include <QCheckBox>
#include <QlineEdit>
#include <MainMenu.h>

class login :
    public QWidget
{
public:
    login();
    bool ButtonPushed();
    bool Prochaine = false;
    Button* NextPage;
    int score = 0;
    int SkinChecked = 0;

private:
    void PlaneCheckBox();
    void ChopperCheckBox();
    void JetCheckBox();
    void NewPlayerCheckBox();
    bool userExists(QString);
    void addUser(QString userName);
    void paintEvent(QPaintEvent* event) override;
    QString* getBestScore(int nombre);
    UserName* Authen;
    QLineEdit* Nom;
    SmallText* New;
    QCheckBox* NewPlayer;
    QCheckBox* Plane;
    QCheckBox* Chopper;
    QCheckBox* Jet;
    bool PlayerIsNew = false;

};

