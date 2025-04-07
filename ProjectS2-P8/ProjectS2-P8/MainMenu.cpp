#include "MainMenu.h"

MainMenu::MainMenu()
{
	setWindowTitle("Menu Principale");
	resize(1920, 1080);

    
	//Bouton commencer le jeu
	NextPage = new Button(this);
	NextPage->setText("Commencer!");
	//NextPage->setGeometry(1360, 865, 150, 50);
    NextPage->setGeometry(1000, 500, 150, 50);          //TEMPORAIRE, POUR QUE JE LE VOIS DANS MON ECRAN -JS
	//Bouton pour passer a la page suivante
	BackPage = new Button(this);
	BackPage->setText("Revenir");
	//BackPage->setGeometry(5, 865, 150, 50);   
	BackPage->setGeometry(150, 500, 150, 50);		  //TEMPORAIRE, POUR QUE JE LE VOIS DANS MON ECRAN -JS
    //Afficher 5 meilleurs scores
    QMap <int, QString>  map = getBestScore(5);
    auto it = map.end();
    while (it != map.begin()) {
        --it;
        BestScores += QString("%1 %2 %3k \n\n")
            .arg(QString::number(i++))
            .arg(it.value(), -9, QChar(' '))
            .arg(QString::number(it.key()));
    }
    TextScore = new UserName(BestScores, this);
    TextScore->setGeometry(1055, 200, 1000, 500);
    TextScore->setAlignment(Qt::AlignLeft | Qt::AlignTop);
}

void MainMenu::paintEvent(QPaintEvent* event)
{

    QPainter painter(this);

    //Image de fond
    QPixmap fond("sprites/background/Sky.png");
    painter.drawPixmap(0, 0, width(), height(), fond);

	//Image Tour de control
	QPixmap tower("sprites/background/ControlTower.png");
	painter.drawPixmap(0, 175, 175, 725, tower);

	//Image aeroport
	QPixmap airport("sprites/background/Airport.png");
	painter.drawPixmap(0, 650, 624, 242, airport);

	//Image de la piste
	QPixmap runway("sprites/background/PisteComplete.png");
	painter.drawPixmap(0, 850, runway);

	//Image avion
	QPixmap avion("sprites/avion/avion1.png");
	painter.drawPixmap(150, 795, 200, 150, avion);

	//Image du tableau des scores
    QPixmap bord("sprites/background/LeaderbordTableau.png");
    painter.drawPixmap(1050, 160, 450, 550, bord);
	QPixmap crown("sprites/background/LeaderbordCrown.png");
	painter.drawPixmap(1050, 10, 450, 150, crown);


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