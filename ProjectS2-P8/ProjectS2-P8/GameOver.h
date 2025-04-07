#pragma once
#include <Qwidget>
#include <Button.h>
#include <UserName.h>
#include <SmallText.h>
#include <QCheckBox>
#include <QlineEdit>
#include <MainMenu.h>
class GameOver :
    public QWidget
{
public:
    GameOver();
    Button* NextPage;
private:
	void paintEvent(QPaintEvent* event) override;

};

