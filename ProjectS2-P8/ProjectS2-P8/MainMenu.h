#pragma once
#include "login.h"
#include <QMessageBox>
#include <QFile>
#include <QString>
#include <QMap>

class MainMenu :
    public QWidget
{
public:
    MainMenu();
    Button* NextPage;
    Button* BackPage;

private:
    UserName* TextScore;
    QString BestScores;
    QString Bonjour;
    int i = 1;
    void paintEvent(QPaintEvent* event) override;
    QMap<int, QString> getBestScore(int nombre);
};
