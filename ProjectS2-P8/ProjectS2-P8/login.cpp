#include "login.h"
#include <QFile>
#include <QMessageBox>

login::login()
{
    setWindowTitle("Authentification");
    //resize(1920, 1080);
    
    //Image de fond
    

    //Texte Entrez votre nom:
    Authen = new UserName("Entrez votre nom:", this, 20, TITLE);
    Authen->setGeometry(430, 250, 400, 35);
   
    //Zone d'ecriture du nom
    Nom = new QLineEdit(this);
    Nom->setGeometry(432, 295, 680, 35);
    Nom->setStyleSheet("QLineEdit {"
        "font-size: 25px;"
        "font-family: Arial;"
        "font-weight: bold;"
        "color: black;"
        "border: 2px solid black;"
        "background: rgba(194, 255, 255, 1);""}");

    //Texte Nouveau joueur:
    New = new UserName("Nouveau joueur?", this, 10, TEXTE);
    New->setGeometry(433, 329, 165, 20);

    //Creation boite a cocher pour savoir si nouveau joueur
    NewPlayer = new QCheckBox(this);
    NewPlayer->move(600, 327);
    NewPlayer->resize(30, 30);

    //Creation boite a cocher pour savoir si nouveau joueur
    Plane = new QCheckBox(this);
    Plane->move(450, 590);
    Plane->resize(30, 30);

    //Creation boite a cocher pour savoir si nouveau joueur
    Chopper = new QCheckBox(this);
    Chopper->move(755, 590);
    Chopper->resize(30, 30);

    //Creation boite a cocher pour savoir si nouveau joueur
    Jet = new QCheckBox(this);
    Jet->move(1080, 590);
    Jet->resize(30, 30);

    //Bouton pour passer a la page suivante
    NextPage = new Button(this);
    NextPage->setText("Confirmer");
    NextPage->setGeometry(680, 700, 150, 50);

    //Connect les checkboxs de skin avec leur fonction 
    connect(Plane, &QCheckBox::clicked, this, &login::PlaneCheckBox);
    connect(Chopper, &QCheckBox::clicked, this, &login::ChopperCheckBox);
    connect(Jet, &QCheckBox::clicked, this, &login::JetCheckBox);
    
    //Connect checkbox de NewPlayer avec sa fonction
    connect(NewPlayer, &QCheckBox::clicked, this, &login::NewPlayerCheckBox);
 }

void login::PlaneCheckBox()
{
    Plane->setChecked(true);
    Chopper->setChecked(false);
    Jet->setChecked(false);
    SkinChecked = 1;
    return;
}

void login::ChopperCheckBox()
{
    Plane->setChecked(false);
    Chopper->setChecked(true);
    Jet->setChecked(false);
    SkinChecked = 2;
    return;
}

void login::JetCheckBox()
{
    Plane->setChecked(false);
    Chopper->setChecked(false);
    Jet->setChecked(true);
    SkinChecked = 3;
    return;
}

void login::NewPlayerCheckBox()
{
    if (NewPlayer->isChecked())
        PlayerIsNew = true;
    else
        PlayerIsNew = false;
    return;
}

bool login::ButtonPushed()
{
    bool userExist = userExists(Nom->text());
    if (SkinChecked == 0)
    {
        QMessageBox::critical(nullptr, "Erreur", "Choisir un moyen de transport avant de continuer!");
        return false;
    }
    else if (PlayerIsNew && !userExist)
    {
        addUser(Nom->text());
        userExists(Nom->text());
        //QMessageBox::information(nullptr, "Information", "Bienvenue : " + Nom->text() + "Vos points : ");
        return true;
    }
    else if (PlayerIsNew && userExist) {
        QMessageBox::critical(nullptr, "Erreur", "Nom d'utilisateur existe deja\n Veuillez vous connecter ou choisir un autre nom");
        return false;

    }
    else if (!PlayerIsNew && userExist) {
        //QMessageBox::information(nullptr, "Information", "content de vous revoir :" + Nom->text() + "Vos points : ");
        return true;
    } 
    else if (!PlayerIsNew && !userExist){
        QMessageBox::critical(nullptr, "Erreur", "Nom d'utilisateur n'existe pas\n Veuillez reessayer ou choisir un autre nom");
        return false;
    }
    else {
        QMessageBox::critical(nullptr, "Erreur", "Probleme de logique");
        return false;
    }
}
bool login::userExists(QString userName) {

    QFile file("DataBase.csv");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Erreur", "Impossible d'ouvrir le fichier !");
        return 1;
    }
    QTextStream in(&file);

    while (!in.atEnd()) {
        QString line = in.readLine();
        QStringList elements = line.split(",");
        if (elements.size() == 2) {
            QString nom = elements[0].trimmed();

            if (nom == userName) {
                score = elements[1].trimmed().toInt();
                file.close();
                return true;
            }
        }
    }
    file.close();
    return false;
}
void login::addUser(QString userName) {
    QFile file("DataBase.csv");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Append | QIODevice::Text)) {
        QMessageBox::critical(nullptr, "Erreur addUser", "Impossible d'ouvrir le fichier !");
    }
    file.write(userName.toUtf8() + ",");
    file.write("0\n");
    file.flush();
    file.close();
}

//Definition de l'image de background
void login::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);

    //Image de fond
    QPixmap fond("sprites/background/LoginPage.png");
    painter.drawPixmap(0, 0, width(), height(), fond);

    //Image plane
    QPixmap plane("sprites/avion/avion1.png");
    painter.drawPixmap(355, 430, 200, 150, plane);

    //Image chopper
    QPixmap chopper("sprites/avion/chopper.png");
    painter.drawPixmap(635, 460, 230, 120, chopper);

    //Image jet
    QPixmap jet("sprites/avion/jet.png");
    painter.drawPixmap(950, 460, 230, 120, jet);

    //On fini le paint event
    QWidget::paintEvent(event);
}



