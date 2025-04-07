#pragma once
#include <QLabel>
#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QFontDatabase>
enum Type {
    TITLE,
    TEXTE
};

class UserName: public QLabel
{
    Q_OBJECT

public:
    UserName(const QString text = "Ici", QWidget* parent = nullptr, int size = 20, Type type = TEXTE, Qt::GlobalColor c = Qt::white) : QLabel(parent) {
        setAlignment(Qt::AlignCenter);
        setText(text);
        m_size = size;
        color = c;
        int fontId = -1;
        if (type == TITLE)
            fontId = QFontDatabase::addApplicationFont("sprites/font/Title.ttf");
        else if (type == TEXTE)
            fontId = QFontDatabase::addApplicationFont("sprites/font/Texte.otf");
        // ou "./MyFont.ttf"
        if (fontId != -1) {
            QStringList loadedFonts = QFontDatabase::applicationFontFamilies(fontId);
            if (!loadedFonts.isEmpty()) {
                customFontFamily = loadedFonts.at(0);
            }
        }
        else {
            qWarning() << "Erreur de chargement de la police personnalisée.";
        }
    }

protected:
    void paintEvent(QPaintEvent* event) override {
        Q_UNUSED(event);
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        painter.setPen(QPen(color));

        //QFont font(customFontFamily, m_size);
        // Set text color (white) and font
        QFont font(customFontFamily, m_size);       
        font.setStyleHint(QFont::Monospace);
        font.setBold(true);               //Bold
        painter.setFont(font);            //Confirme la font pour l'ecriture

        //Ecriture au centre
        painter.drawText(rect(), Qt::AlignLeft, text());
    }
private:
    int m_size;
    QString customFontFamily;
    Qt::GlobalColor color;
};

