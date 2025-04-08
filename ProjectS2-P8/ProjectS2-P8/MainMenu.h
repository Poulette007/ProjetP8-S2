#pragma once
#include "login.h"
#include "Stat.h"
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QMap>
#include "ImageManager.h"

class MainMenu :
    public QWidget
{
public:
    MainMenu();
    Button* NextPage;
    Button* BackPage;

private:
    UserName* TextScore;
    UserName* Welcome;
    QString BestScores;
    QString Bonjour;
    int i = 1;
    void paintEvent(QPaintEvent* event) override;
    QMap<int, QString> getBestScore(int nombre);
};
