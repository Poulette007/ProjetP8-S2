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

/*---------------------------- Variables globales ---------------------------*/

volatile bool shouldSend_ = false;  // Drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // Drapeau prêt à lire un message

int ledState = 0;
int pinLED = 7;

//Moteur vibrant
int moteur_vibration = 42;

//DEL bleue
int DEL_PIN = 33;

//Joystick
int joystickY = A1;
int joystickYValue = 0;

int joystickX = A2;
int joystickXValue= 0;

int joystickTresh = 10;

bool clavierHaut = false;
bool clavierBas = false;
bool clavierDroit = false;
bool clavierGauche = false;
bool jeuHaut = false;
bool jeuBas = false;

//potentiometre
int potValue = 0;
int potTresh = 5;
int pinPOT = A0;

//Bouton poussoir
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

//LCD
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


//bool
bool trigger = false;

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
void testDEL();
void demoAudit2();
/*---------------------------- Fonctions "Main" -----------------------------*/

void setup() {
  Serial.begin(BAUD);               // Initialisation de la communication serielle
  pinMode(pinLED, OUTPUT);
  digitalWrite(pinLED, ledState);

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

  //DEL
  pinMode(DEL_PIN, OUTPUT);
}

/* Boucle principale (infinie) */
void loop() {

  if(shouldRead_){
    readMsg();
    sendMsg();
  }

  testPotentiometre();
  potValue = analogRead(pinPOT);

  testBouton();

  lectureAccelerometre();
  pinXValue = analogRead(pinX);


  joystickXValue = analogRead(joystickX);
  joystickYValue = analogRead(joystickY);
  testJoystick();

  delay(50);  // delais de 10 ms
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
  /*if((joystickXValue - analogRead(joystickX))>= joystickTresh){
    trigger = true;
  }
  if((analogRead(joystickX) - joystickXValue)>= joystickTresh){
    trigger = true;
  }
  if((joystickYValue - analogRead(joystickY))>= joystickTresh){
    trigger = true;
  }
  if((analogRead(joystickY) - joystickYValue)>= joystickTresh){
    trigger = true;
  }*/
  if(!clavierBas && !clavierDroit && !clavierGauche && !clavierHaut && !jeuBas && !jeuHaut)
  {
    if(joystickYValue > 900 && joystickXValue > 150 && joystickXValue < 800)
    {
      clavierHaut = true;
      trigger = true;
    }
    if(joystickYValue < 100 && joystickXValue > 200 && joystickXValue < 800)
    {
      clavierBas = true;
      trigger = true;
    }
    if(joystickXValue > 900 && joystickYValue > 200 && joystickYValue < 800)
    {
      clavierGauche = true;
      trigger = true;
    }
    if(joystickXValue < 100 && joystickYValue > 200 && joystickYValue < 800)
    {
      clavierDroit = true;
      trigger = true;
    }
    if(joystickYValue > 850)
    {
      jeuHaut = true;
      trigger = true;
    }
    if(joystickYValue < 150)
    {
      jeuBas = true;
      trigger = true;
    }
  }
  if( joystickXValue < 800 && joystickXValue > 200 &&  joystickYValue < 800 && joystickYValue > 200)
  {
    clavierBas = false;
    clavierDroit = false;
    clavierGauche = false;
    clavierHaut = false;

    jeuBas = false;
    jeuHaut = false;
  }
}

/*---------------------------Definition de fonctions ------------------------
Fonction d'accélérometre
Sortie : valeur X,Y,Z
-----------------------------------------------------------------------------*/

void lectureAccelerometre(){

  if((350 >= analogRead(pinX) && analogRead(pinX)>= 302) && angleAvion != 2)
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


/*---------------------------Definition de fonctions ------------------------
Fonction d'automatisation des tests pour l'audit
Sortie : affichage de messages à l'écran
-----------------------------------------------------------------------------*/
void demoAudit2()
{
  testEcranLCDs("Accelerometre", 0, 0);
  for (int i=0; i<100; i++){
    lectureAccelerometre();
    delay(100);
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test du bargraph", 0, 0);
  for (int i = 0; i < 4; i++){
    testBargraph();
    delay(1000);
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test des boutons", 0, 0);
  while (!NORD || !SUD || !EST || !OUEST){
    testBouton();
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test du pot", 0, 0);
  for (int i=0; i<100; i++){
    testPotentiometre();
    delay(100);
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test du joystick", 0, 0);
  delay(1000);
  for (int i=0; i<100; i++){
    testJoystick();
    delay(100);
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test du moteur", 0, 0);
  for (int i=0; i<2; i++){
    testMoteurVibrant();
    delay(100);
  }
  delay(1000);
  lcd.clear();

  testEcranLCDs("Test DEL bleue", 0, 0);
  for (int i=0; i<2; i++){
    testDEL();
    delay(100);
  }  
  delay(1000);
  lcd.clear();

  testEcranLCDs("Fin des tests!!!", 0, 0);
  delay(1000);
  lcd.clear();
}


/*---------------------------Definition de fonctions ------------------------
Fonction d'envoi
Entrée : Aucun
Sortie : Aucun
Traitement : Envoi du message
-----------------------------------------------------------------------------*/
void sendMsg() {
  StaticJsonDocument<500> doc;
  // Elements du message
  // doc["time"] = millis();
  // doc["analog"] = potValue;
  doc["potentio"] = potValue;
  doc["BoutonGauche"] = BoutonGaucheValue;
  doc["BoutonDroit"] = BoutonDroitValue;
  doc["BoutonBas"] = BoutonBasValue;
  doc["BoutonHaut"] = BoutonHautValue;

  //tests des valeur
  //doc["joystickX"] = joystickXValue;
  //doc["joystickY"] = joystickYValue;

  doc["clavierGauche"] = clavierGauche;
  doc["clavierDroit"] = clavierDroit;
  doc["clavierBas"] = clavierBas;
  doc["clavierHaut"] = clavierHaut;
  doc["jeuBas"] = jeuBas;
  doc["jeuHaut"] = jeuHaut;

  doc["accelX"] = angleAvion; 


  // JsonArray array = doc.createNestedArray("vecteur");
  // for (int i = 0; i < 20; i++) {
  //   array.add(vecteur[i]);
  // }
  // Serialisation


  // Envoie
  if(trigger){
    serializeJson(doc, Serial);
    Serial.println();
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
  JsonVariant parse_msg;

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
    digitalWrite(pinLED,doc["led"].as<bool>());
  }
}