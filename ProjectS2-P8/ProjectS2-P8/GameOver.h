#pragma once
#include <QApplication>
#include <Qwidget>
#include <Button.h>
#include <UserName.h>
#include <SmallText.h>
#include <QCheckBox>
#include <QlineEdit>
class GameOver :
    public QWidget
{
public:
	GameOver(bool victoire);
	Button* Retour;
	bool ButtonPushed();
	void setVictoire(bool);
	bool estVictoire;
private:
	
	void paintEvent(QPaintEvent* event) override;

};

