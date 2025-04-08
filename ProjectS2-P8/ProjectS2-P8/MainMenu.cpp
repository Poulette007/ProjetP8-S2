#include "MainMenu.h"

MainMenu::MainMenu()
{
    setWindowTitle("Menu Principale");
    resize(1920, 1080);

    //Afficher 5 meilleurs scores
    QMap <int, QString>  map = getBestScore(5);
    auto it = map.end();
    while (it != map.begin()) {
        --it;
        BestScores += QString("%1%2 %3k \n\n\n")
            .arg(QString::number(i++))
            .arg(it.value(), -10, QChar('\t'))
            .arg(QString::number(it.key()/1000));
    }
    TextScore = new UserName(BestScores, this, 25, TEXTE, Qt::black);
    TextScore->setGeometry(1070, 157, 1000, 550);
    TextScore->setAlignment(Qt::AlignLeft | Qt::AlignTop);

    //Message de bienvenu
    Bonjour = QString("Bonjour %1 \nMeilleur socre: %2")
        .arg(Stat::playerName)
        .arg(QString::number(Stat::previousScore));
    Welcome = new UserName(Bonjour, this, 35, TEXTE, Qt::white);
    Welcome->setGeometry(0, 0, 950, 105);

    //Bouton commencer le jeu
    NextPage = new Button(this);
    NextPage->setText("Commencer!");
    NextPage->setGeometry(350, 150, 150, 50);

    //Bouton pour passer a la page suivante
    BackPage = new Button(this);
    BackPage->setText("Revenir");
    BackPage->setGeometry(50, 150, 150, 50);
}

void MainMenu::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //Image de fond
    QPixmap fond("sprites/background/BackgroundDecoAtte.png");
    painter.drawPixmap(0, 0, width(), height(), fond);

    //Image aeroport
    QPixmap airport = ImageManager::getInstance().getImage(AIRPORT);
    painter.drawPixmap(350, 270, 650, 550, airport);

    //Image de la piste
    QPixmap runway("sprites/background/PisteComplete.png");
    painter.drawPixmap(0, 693, 4000, 150, runway);

    //Image avion
    if (login::SkinChecked == 1)
    {
        QPixmap avion("sprites/avion/avion1.png");
        painter.drawPixmap(0, 670, 240, 125, avion);
    }
    else if (login::SkinChecked == 2)
    {
        QPixmap chopper("sprites/avion/chopper.png");
        painter.drawPixmap(0, 670, 240, 125, chopper);
    }
    else if (login::SkinChecked == 3)
    {
        QPixmap jet("sprites/avion/jet.png");
        painter.drawPixmap(0, 670, 240, 125, jet);
    }


    //Image du tableau des scores
    QPixmap bord("sprites/background/LeaderbordTableau.png");
    painter.drawPixmap(1050, 150, 450, 550, bord);
    QPixmap crown("sprites/background/LeaderbordCrown.png");
    painter.drawPixmap(1050, 0, 450, 150, crown);

    qDebug() << Stat::playerName;
    Welcome->setText(QString("Bonjour %1 \nMeilleur socre: %2")
        .arg(Stat::playerName)
        .arg(QString::number(Stat::previousScore)));
    //On fini le paint event
    QWidget::paintEvent(event);

}

/* Comment les afficher:
QMap <int, QString>  map = getBestScore(5);
    for (auto it = map.end(); it != map.begin(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
*/
QMap<int, QString> MainMenu::getBestScore(int nombre)
{
    QMap<int, QString> topScore;
    QFile file("DataBase.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier !");
        return topScore;
    }
    QTextStream in(&file);


    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList elements = line.split(",");
        QString nom = elements[0].trimmed();
        int score = elements[1].trimmed().toInt();
        if (topScore.size() >= 5)
        {
            if (topScore.firstKey() < score) {
                topScore.remove(topScore.firstKey());
                topScore.insert(score, nom);
            }
        }
        else
            topScore.insert(score, nom);
    }
    return topScore;
}