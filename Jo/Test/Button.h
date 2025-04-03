#pragma once
#include <QPushButton>
#include <QPainter>
#include <QStyleOptionButton>
#include <QPalette>
#include <QMouseEvent>

class Button :
    public QPushButton
{
    Q_OBJECT

public:
    Button(QWidget* parent = nullptr) : QPushButton(parent) {
        setAutoDefault(false);
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        QPainter painter(this);
        QStyleOptionButton option;
        option.initFrom(this);
        
        //Selon l'etat du boutton, on a des comportement different
        if (Pressed) {
            painter.setBrush(QBrush(Qt::green));  // Darker color when pressed
        }
        else if (underMouse()) {
            painter.setBrush(QColor(170, 255, 255));  // Lighter color when hovered
        }
        else {
            painter.setBrush(QBrush(Qt::lightGray));  // Normal color
        }
        
        

        QPen borderPen(QColor(0, 0, 0), 5);  
        borderPen.setStyle(Qt::SolidLine);  
        painter.setPen(borderPen);
        painter.drawRect(0, 0, width(), height());

        // Draw the text in the center of the button
        painter.setPen(QPen(Qt::black));
        QFont font("Arial", 14);
        font.setBold(true);
        painter.setFont(font);
        QRect textRect = this->rect();
        painter.drawText(textRect, Qt::AlignCenter, text());

        

        //QPushButton::paintEvent(event); 
    }

    void mousePressEvent(QMouseEvent* event) override {
        //Quand le boutton est appuye
        if (event->button() == Qt::LeftButton) {
            Pressed = true;
            update(); 
        }

        //Pour garder le reste des fonctions
        QPushButton::mousePressEvent(event);
    }

    void mouseReleaseEvent(QMouseEvent* event) override {
        //Quand le boutton n'est pas appuye
        if (event->button() == Qt::LeftButton) {
            Pressed = false;
            update();
        }

        //Pour garder le reste des fonctions
        QPushButton::mouseReleaseEvent(event);
    }

private:
    bool Pressed = false;
};


