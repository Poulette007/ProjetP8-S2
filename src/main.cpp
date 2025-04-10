/* 
 * Auteurs: Jean-Samuel Lauzon    
 * Date: Fevrier 2022
 * Modif : Janvier 2025, 115200 BAUDS, JpGouin
*/

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include <ArduinoJson.h>
#include <LiquidCrystal.h>
//#include <string.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 115200        // Frequence de transmission serielle
//#define BAUD 9600        // Frequence de transmission serielle

/*---------------------------- Variables globales ---------------------------*/

volatile bool shouldSend_ = false;  // Drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // Drapeau prêt à lire un message

//Moteur vibrant
int moteur_vibration = 42;

//DEL bleue
int DEL_PIN = 33;
int ledState = 0;

//Joystick
int joystickY = A1;
int joystickYValue = 0;

int joystickX = A2;
int joystickXValue= 0;

int joystickTresh = 10;

bool clavierHaut = 0;
bool clavierBas = 0;
bool clavierDroit = 0;
bool clavierGauche = 0;
bool jeuHaut = 0;
bool jeuBas = 0;

//potentiometre
int potValue = 0;
int potTresh = 5;
int pinPOT = A0;

//Boutons poussoir
int BoutonGauche = 50;
int BoutonDroit = 51;
int BoutonBas = 52;
int BoutonHaut = 53;

bool NORD = false;
bool EST = false;
bool SUD = false;
bool OUEST = false;

int BoutonGaucheValue = 0;
int BoutonDroitValue = 0;
int BoutonBasValue = 0;
int BoutonHautValue = 0;

//Accelerometre
int pinX = A5;
int pinY = A3;
int pinZ = A4;

int pinXValue = 0;
int pinYValue = 0;
int pinZValue = 0;

int accTresh = 3;

int angleAvion = 0;
int lastAccelValue = 0;
int vecteur[20] = {0};

//bargraph
int bar1 = 22;
int bar2 = 24;
int bar3 = 26;
int bar4 = 28;
int bar5 = 30;
int bar6 = 32;
int bar7 = 34;
int bar8 = 36;
int bar9 = 38;
int bar10 = 40;

// Débounce pour les boutons
unsigned long lastDebounceTimeBG = 0;
unsigned long lastDebounceTimeBD = 0;
unsigned long lastDebounceTimeBB = 0;
unsigned long lastDebounceTimeBH = 0;
const int debounceDelay = 10;  // Délai de debounce en millisecondes

//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Muons
int pinMuon = A8;
int triggerMuon = 517;
bool valeurMuon = false;
int amplitutdeMuon = 0;



//bool
bool trigger = false;
bool shaking = false;

/*------------------------- Prototypes de fonctions -------------------------*/
void sendMsg(); 
void readMsg();
void serialEvent();
void lectureAccelerometre();
void testBargraph();
void testBouton();
void testPotentiometre();
void testJoystick();
void testMoteurVibrant();
void testEcranLCDs(String info, int curseurX, int curseurY);
void testEcranLCDi(int info, int curseurX, int curseurY);
void detecteurMuon();
/*---------------------------- Fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation de la communication serielle
  
  //Del
  pinMode(DEL_PIN, OUTPUT);
  digitalWrite(DEL_PIN, ledState);

  //bargraph
  pinMode(bar1, OUTPUT);
  pinMode(bar2, OUTPUT);
  pinMode(bar3, OUTPUT);
  pinMode(bar4, OUTPUT);
  pinMode(bar5, OUTPUT);
  pinMode(bar6, OUTPUT);
  pinMode(bar7, OUTPUT);
  pinMode(bar8, OUTPUT);
  pinMode(bar9, OUTPUT);
  pinMode(bar10, OUTPUT);

  //bouton
  pinMode(BoutonGauche, INPUT_PULLUP);
  pinMode(BoutonDroit, INPUT_PULLUP);
  pinMode(BoutonBas, INPUT_PULLUP);
  pinMode(BoutonHaut, INPUT_PULLUP);

  //Moteur vibrant
  pinMode(moteur_vibration, OUTPUT);

  //Écran LCD
  lcd.begin(16, 2);
  // Print a message to the LCD.
  //lcd.print("hello, world!");
}

/* Boucle principale (infinie) */
void loop() {

  if(shouldRead_){
    readMsg();
  }
  detecteurMuon();
  sendMsg();
  
  testPotentiometre();
  potValue = analogRead(pinPOT);

  testBouton();

  
  lectureAccelerometre();
  pinXValue = analogRead(pinX);

  
  joystickXValue = analogRead(joystickX);
  joystickYValue = analogRead(joystickY);
  testJoystick();
  //faut que le arduino soit plus lent que l'ordi
  delay(90);  
}

/*---------------------------Definition de fonctions ------------------------*/

void serialEvent() { shouldRead_ = true; }

/*---------------------------Definition de fonctions ------------------------
Fonction du test potentiometre
Sortie : valeur de la résistance
-----------------------------------------------------------------------------*/

void testPotentiometre(){
  // potValue = analogRead(pinPOT);
  // testEcranLCDs("Valeur potentiomètre", 0,0);
  // testEcranLCDi(potValue, 5, 1);
  if((potValue - analogRead(pinPOT))>= potTresh){
    trigger = true;
  }
  if((analogRead(pinPOT) - potValue)>= potTresh){
    trigger = true;
  }

}


/*---------------------------Definition de fonctions ------------------------
Fonction du test bouton
Sortie : état du bouton
-----------------------------------------------------------------------------*/

void testBouton(){

  /**/
  //old version
  if(!(BoutonGaucheValue = digitalRead(BoutonGauche)))
    trigger = true;
  if(!(BoutonDroitValue = digitalRead(BoutonDroit)))
    trigger = true;
  if(!(BoutonBasValue = digitalRead(BoutonBas)))
    trigger = true;
  if(!(BoutonHautValue = digitalRead(BoutonHaut)))
    trigger = true;
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test Joystick
Sortie : valeur de la résistance en X et en Y
-----------------------------------------------------------------------------*/

void testJoystick(){
  //tests des valeurs du joystick

  if(!clavierBas && !clavierDroit && !clavierGauche && !clavierHaut && !jeuBas && !jeuHaut)
  {
    /*if(joystickYValue > 900 && joystickXValue > 150 && joystickXValue < 800)
    {
      clavierHaut = 1;
      trigger = true;
    }
    if(joystickYValue < 100 && joystickXValue > 200 && joystickXValue < 800)
    {
      clavierBas = 1;
      trigger = true;
    }
    if(joystickXValue > 900 && joystickYValue > 200 && joystickYValue < 800)
    {
      clavierGauche = 1;
      trigger = true;
    }*/
    if(joystickXValue < 100 && joystickYValue > 200 && joystickYValue < 800)
    {
      clavierDroit = 1;
      trigger = true;
    }
    if(joystickYValue > 750)
    {
      jeuHaut = 1;
      trigger = true;
    }
    if(joystickYValue < 250)
    {
      jeuBas = 1;
      trigger = true;
    }
  }
  if( joystickXValue < 750 && joystickXValue > 250 &&  joystickYValue < 750 && joystickYValue > 250)
  {
    clavierBas = 0;
    clavierDroit = 0;
    clavierGauche = 0;
    clavierHaut = 0;

    jeuBas = 0;
    jeuHaut = 0;
  }
}

/*---------------------------Definition de fonctions ------------------------
Fonction d'accélérometre
Sortie : valeur X,Y,Z
-----------------------------------------------------------------------------*/

void lectureAccelerometre(){

  /*if((350 >= analogRead(pinX) && analogRead(pinX)>= 302) && angleAvion != 2)
  {
    trigger = true;
    angleAvion = 2;

  }
  //analogRead(pinX) - pinXValue) >= accTresh
  if((297 >= analogRead(pinX) && analogRead(pinX)>= 278)&& angleAvion != 1)
  {
    trigger = true;
    angleAvion = 1;

  }
  if((274 >= analogRead(pinX) && analogRead(pinX)>= 250)&& angleAvion != 0)
  {
    trigger = true;
    angleAvion = 0;
  }*/
  int accelValue = analogRead(pinX);
  if(abs(accelValue - lastAccelValue)>=35)
  {
    
    shaking = true;
    //Serial.println("Shaking detected!");
    lastAccelValue = accelValue;
    trigger = true;

  }
  else
  {
    
    shaking = false;
  }
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test bargraph
Sortie : lumière
-----------------------------------------------------------------------------*/

void testBargraph(){
  digitalWrite(bar1, HIGH);
  delay(100);  // Attendre avant d'allumer la suivante
  digitalWrite(bar2, HIGH);
  delay(100);
  digitalWrite(bar3, HIGH);
  delay(100);
  digitalWrite(bar4, HIGH);
  delay(100);
  digitalWrite(bar5, HIGH);
  delay(100);
  digitalWrite(bar6, HIGH);
  delay(100);
  digitalWrite(bar7, HIGH);
  delay(100);
  digitalWrite(bar8, HIGH);
  delay(100);
  digitalWrite(bar9, HIGH);
  delay(100);
  digitalWrite(bar10, HIGH);
  delay(100);


  digitalWrite(bar1, LOW);
  delay(100);  // Attendre avant d'allumer la suivante
  digitalWrite(bar2, LOW);
  delay(100);
  digitalWrite(bar3, LOW);
  delay(100);
  digitalWrite(bar4, LOW);
  delay(100);
  digitalWrite(bar5, LOW);
  delay(100);
  digitalWrite(bar6, LOW);
  delay(100);
  digitalWrite(bar7, LOW);
  delay(100);
  digitalWrite(bar8, LOW);
  delay(100);
  digitalWrite(bar9, LOW);
  delay(100);
  digitalWrite(bar10, LOW);
  delay(100);

}


/*---------------------------Definition de fonctions ------------------------
Fonction du test moteur vibrant
Sortie : vibration
-----------------------------------------------------------------------------*/
void testMoteurVibrant(){
  digitalWrite(moteur_vibration, HIGH);
  testEcranLCDs("Activee         ", 1, 1);
  delay(1000);
  lcd.clear();
  digitalWrite(moteur_vibration, LOW);
  testEcranLCDs("Desactivee      ", 1, 1);
  delay(1000);
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test le l'écran LCD
Sortie : affichage du message à l'écran
-----------------------------------------------------------------------------*/
void testEcranLCDs(String info, int curseurX, int curseurY){
  lcd.setCursor(curseurX, curseurY);
  lcd.print(info);
}

void testEcranLCDi(int info, int curseurX, int curseurY){
  lcd.setCursor(curseurX, curseurY);
  lcd.print(info);
}

void detecteurMuon(){

  //valeurMuon = false;
  amplitutdeMuon = analogRead(pinMuon);

  if(amplitutdeMuon >= triggerMuon){
    valeurMuon = !valeurMuon;
    trigger = true;
  }

  //Serial.println(amplitutdeMuon);
}
/*---------------------------Definition de fonctions ------------------------
Fonction d'envoi
Entrée : Aucun
Sortie : Aucun
Traitement : Envoi du message
-----------------------------------------------------------------------------*/
void sendMsg() {
  StaticJsonDocument<1024> doc;
  while (Serial.available() > 0) { Serial.read(); }
  // Elements du message
  // doc["time"] = millis();
  // doc["analog"] = potValue;
  doc["pot"] = potValue;
  doc["BG"] = BoutonGaucheValue;
  doc["BD"] = BoutonDroitValue;
  doc["BB"] = BoutonBasValue;
  doc["BH"] = BoutonHautValue;

  //tests des valeur
  //doc["joystickX"] = joystickXValue;
  //doc["joystickY"] = joystickYValue;

  doc["CG"] = clavierGauche;
  doc["CD"] = clavierDroit;
  doc["CB"] = clavierBas;
  doc["CH"] = clavierHaut;
  doc["JB"] = jeuBas;
  doc["JH"] = jeuHaut;

  //doc["AX"] = angleAvion; 
  doc["AX"] = shaking;
  doc["Muon"]  = valeurMuon;

  // JsonArray array = doc.createNestedArray("vecteur");
  // for (int i = 0; i < 20; i++) {
  //   array.add(vecteur[i]);
  // }
  // Serialisation


  // Envoie
  if(trigger){
    
    serializeJson(doc, Serial);
    Serial.println();
    valeurMuon = false;
    trigger = false;
    shouldSend_ = false;
  }
  
}

/*---------------------------Definition de fonctions ------------------------
Fonction de reception
Entrée : Aucun
Sortie : Aucun
Traitement : Réception du message
-----------------------------------------------------------------------------*/
void readMsg(){
  // Lecture du message Json
  StaticJsonDocument<500> doc;
  JsonVariant parse_msg, bargraph, lcda, moteur;

  // Lecture sur le port Seriel
  DeserializationError error = deserializeJson(doc, Serial);
  shouldRead_ = false;

  // Si erreur dans le message
  if (error) {
    Serial.print("deserialize() failed: ");
    Serial.println(error.c_str());
    return;
  }
  
  // Analyse des éléments du message message
  parse_msg = doc["led"];
  if (!parse_msg.isNull()) {
    // mettre la led a la valeur doc["led"]
    digitalWrite(DEL_PIN,doc["led"].as<bool>());
  }

  bargraph = doc["BAR"];
  if (bargraph <= 10 && bargraph >= 0)
  {
    if(bargraph == 10)
    {
      digitalWrite(bar1, HIGH);
      digitalWrite(bar2, HIGH);
      digitalWrite(bar3, HIGH);
      digitalWrite(bar4, HIGH);
      digitalWrite(bar5, HIGH);
      digitalWrite(bar6, HIGH);
      digitalWrite(bar7, HIGH);
      digitalWrite(bar8, HIGH);
      digitalWrite(bar9, HIGH);
      digitalWrite(bar10, HIGH);
    }
    else if(bargraph == 9)
    {
      digitalWrite(bar1, 1);
      digitalWrite(bar2, 1);
      digitalWrite(bar3, 1);
      digitalWrite(bar4, 1);
      digitalWrite(bar5, 1);
      digitalWrite(bar6, 1);
      digitalWrite(bar7, 1);
      digitalWrite(bar8, 1);
      digitalWrite(bar9, 1);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 8)
    {
      digitalWrite(bar1, 1);
      digitalWrite(bar2, 1);
      digitalWrite(bar3, 1);
      digitalWrite(bar4, 1);
      digitalWrite(bar5, 1);
      digitalWrite(bar6, 1);
      digitalWrite(bar7, 1);
      digitalWrite(bar8, 1);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 7)
    {
      digitalWrite(bar1, 1);
      digitalWrite(bar2, 1);
      digitalWrite(bar3, 1);
      digitalWrite(bar4, 1);
      digitalWrite(bar5, 1);
      digitalWrite(bar6, 1);
      digitalWrite(bar7, 1);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 6)
    {
      digitalWrite(bar1, 1);
      digitalWrite(bar2, 1);
      digitalWrite(bar3, 1);
      digitalWrite(bar4, 1);
      digitalWrite(bar5, 1);
      digitalWrite(bar6, 1);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 5)
    {
      digitalWrite(bar1, 1);
      digitalWrite(bar2, 1);
      digitalWrite(bar3, 1);
      digitalWrite(bar4, 1);
      digitalWrite(bar5, 1);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 4)
    {
      digitalWrite(bar1, HIGH);
      digitalWrite(bar2, HIGH);
      digitalWrite(bar3, HIGH);
      digitalWrite(bar4, HIGH);
      digitalWrite(bar5, 0);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 3)
    {
      digitalWrite(bar1, HIGH);
      digitalWrite(bar2, HIGH);
      digitalWrite(bar3, HIGH);
      digitalWrite(bar4, 0);
      digitalWrite(bar5, 0);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 2)
    {
      digitalWrite(bar1, HIGH);
      digitalWrite(bar2, HIGH);
      digitalWrite(bar3, 0);
      digitalWrite(bar4, 0);
      digitalWrite(bar5, 0);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 1)
    {
      digitalWrite(bar1, HIGH);
      digitalWrite(bar2, 0);
      digitalWrite(bar3, 0);
      digitalWrite(bar4, 0);
      digitalWrite(bar5, 0);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
    else if(bargraph == 0)
    {
      digitalWrite(bar1, 0);
      digitalWrite(bar2, 0);
      digitalWrite(bar3, 0);
      digitalWrite(bar4, 0);
      digitalWrite(bar5, 0);
      digitalWrite(bar6, 0);
      digitalWrite(bar7, 0);
      digitalWrite(bar8, 0);
      digitalWrite(bar9, 0);
      digitalWrite(bar10, 0);     
    }
  }
  else
  {
    Serial.println("Json du bargraph en dehors de 0 a 10");
  }

    //LCD JSON
    lcda = doc["LCD"];
    if (!lcda.isNull()){
      lcd.setCursor(0,0);
      lcd.print(doc["LCD"].as<String>());
    }

    //moteur
    moteur = doc["MV"];
    if (!parse_msg.isNull()) {
      digitalWrite(moteur_vibration, doc["MV"].as<bool>());
    }
}