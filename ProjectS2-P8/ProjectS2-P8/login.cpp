#include "login.h"

login::login()
{
    setWindowTitle("Authentification");
    resize(1920, 1080);
    
    //Image de fond
    

    //Texte Entrez votre nom:
    Authen = new UserName("Entrez votre nom:", this);
    Authen->setGeometry(430, 255, 400, 35);
   
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
    New = new SmallText("Nouveau joueur?", this);
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

    //Connect le boutton confirmer
    connect(NextPage, &QPushButton::clicked, this, &login::ButtonPushed);
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

void login::ButtonPushed()
{
    return;
}



