#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_front_end.h"

class front_end : public QMainWindow
{
    Q_OBJECT

public:
    front_end(QWidget *parent = nullptr);
    ~front_end();

private:
    Ui::front_endClass ui;
};
