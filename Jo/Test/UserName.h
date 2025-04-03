#pragma once
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>


class UserName: public QLabel
{
    Q_OBJECT

public:
    UserName(const QString text = "Ici", QWidget* parent = nullptr) : QLabel(parent) {
        setAlignment(Qt::AlignCenter);
        setText(text);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);

        // Set text color (white) and font
        painter.setPen(QPen(Qt::black));  //Couleur
        QFont font("Arial", 35);          //Font et grosseur
        font.setBold(true);               //Bold
        painter.setFont(font);            //Confirme la font pour l'ecriture

        //Ecriture au centre
        painter.drawText(rect(), Qt::AlignCenter, text());
    }
};

