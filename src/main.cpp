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

//potentiometre
int potValue = 0;
int pinPOT = A0;

//Bouton poussoir
int BP1_PIN = 50;
int BP2_PIN = 51;
int BP3_PIN = 52;
int BP4_PIN = 53;
bool NORD = false;
bool EST = false;
bool SUD = false;
bool OUEST = false;

//Accelerometre
int pinX = A5;
int pinY = A3;
int pinZ = A4;

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
  pinMode(BP1_PIN, INPUT_PULLUP);
  pinMode(BP2_PIN, INPUT_PULLUP);
  pinMode(BP3_PIN, INPUT_PULLUP);
  pinMode(BP4_PIN, INPUT_PULLUP);

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

  //demoAudit2();
  // lectureAccelerometre();
  // testBargraph();
  // testBouton();
  // testPotentiometre();
  //testJoystick();
  //testMoteurVibrant();
  //testEcranLCD();

  delay(50000);  // delais de 10 ms
}

/*---------------------------Definition de fonctions ------------------------*/

void serialEvent() { shouldRead_ = true; }

/*---------------------------Definition de fonctions ------------------------
Fonction d'accélérometre
Sortie : valeur X,Y,Z
-----------------------------------------------------------------------------*/

void lectureAccelerometre(){
  testEcranLCDs("x:", 0, 1);
  testEcranLCDi(analogRead(pinX), 2, 1);
  testEcranLCDs("y:", 5, 1);
  testEcranLCDi(analogRead(pinY), 7,1);
  testEcranLCDs("z:", 10, 1);
  testEcranLCDi(analogRead(pinZ), 12, 1);
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
Fonction du test bouton
Sortie : état du bouton
-----------------------------------------------------------------------------*/

void testBouton(){
  /*
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP1_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP2_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP3_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP4_PIN));
  */

  if(!digitalRead(BP1_PIN)){
    testEcranLCDs("Bouton: Ouest   ", 1, 1);
    OUEST = true;
    return;
  }
  else if (!digitalRead(BP2_PIN)){
    testEcranLCDs("Bouton: Est     ", 1, 1);
    EST = true;
    return;
  }
  else if(!digitalRead(BP3_PIN)){
    testEcranLCDs("Bouton: Sud     ", 1, 1);
    SUD = true;
    return;
  }
  else if(!digitalRead(BP4_PIN))
  {
    testEcranLCDs("Bouton: Nord    ", 1, 1);
    NORD = true;
    return;
  }
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test potentiometre
Sortie : valeur de la résistance
-----------------------------------------------------------------------------*/

void testPotentiometre(){
  potValue = analogRead(pinPOT);
  testEcranLCDs("Valeur potentiomètre", 0,0);
  testEcranLCDi(potValue, 5, 1);
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test Joystick
Sortie : valeur de la résistance en X et en Y
-----------------------------------------------------------------------------*/

void testJoystick(){
  joystickYValue = analogRead(joystickY);
  testEcranLCDs("Valeur en Y: ", 0, 1);
  testEcranLCDi(joystickYValue, 13, 1);

  joystickXValue = analogRead(joystickX);
  testEcranLCDs("Valeur en X: ", 0, 0);
  testEcranLCDi(joystickXValue, 13, 0);
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

void testDEL(){
  digitalWrite(DEL_PIN, HIGH);
  testEcranLCDs("Activee         ", 1, 1);
  delay(1000);
  digitalWrite(DEL_PIN, LOW);
  testEcranLCDs("Desactivee      ", 1, 1);
  delay(1000);
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
  doc["time"] = millis();
  doc["analog"] = potValue;

  // Serialisation
  serializeJson(doc, Serial);

  // Envoie
  Serial.println();
  shouldSend_ = false;
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