/* 
 * Auteurs: Jean-Samuel Lauzon    
 * Date: Fevrier 2022
 * Modif : Janvier 2025, 115200 BAUDS, JpGouin
*/

/*------------------------------ Librairies ---------------------------------*/
#include <Arduino.h>
#include <ArduinoJson.h>

/*------------------------------ Constantes ---------------------------------*/

#define BAUD 115200        // Frequence de transmission serielle

/*---------------------------- Variables globales ---------------------------*/

volatile bool shouldSend_ = false;  // Drapeau prêt à envoyer un message
volatile bool shouldRead_ = false;  // Drapeau prêt à lire un message

int ledState = 0;


int pinLED = 7;

//Moteur vibrant
int moteur_vibration = 32;


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


//Accelerometre
int pinX = A5;
int pinY = A3;
int pinZ = A4;

//barnagraphe
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
}

/* Boucle principale (infinie) */
void loop() {

  if(shouldRead_){
    readMsg();
    sendMsg();
  }

  
  // lectureAccelerometre();
  // testBargraph();
  // testBouton();
  // testPotentiometre();
  //testJoystick();
  testMoteurVibrant();

  delay(500);  // delais de 10 ms
}

/*---------------------------Definition de fonctions ------------------------*/

void serialEvent() { shouldRead_ = true; }

/*---------------------------Definition de fonctions ------------------------
Fonction d'accélérometre
Sortie : valeur X,Y,Z
-----------------------------------------------------------------------------*/

void lectureAccelerometre(){
  Serial.print("x: ");
  Serial.print(analogRead(pinX));
  Serial.print(" y: ");
  Serial.print(analogRead(pinY));
  Serial.print(" z: ");
  Serial.println(analogRead(pinZ));
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
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP1_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP2_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP3_PIN));
  Serial.print("Etat bouton 1");
  Serial.println(digitalRead(BP4_PIN));

}

/*---------------------------Definition de fonctions ------------------------
Fonction du test potentiometre
Sortie : valeur de la résistance
-----------------------------------------------------------------------------*/

void testPotentiometre(){
  potValue = analogRead(pinPOT);
  Serial.print("Valeur potentiomètre");
  Serial.println(potValue);
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test Joystick
Sortie : valeur de la résistance en X et en Y
-----------------------------------------------------------------------------*/

void testJoystick(){
  joystickYValue = analogRead(joystickY);
  Serial.print("Valeur en Y: ");
  Serial.println(joystickYValue);

  joystickXValue = analogRead(joystickX);
  Serial.print("Valeur en X: ");
  Serial.println(joystickXValue);
}

/*---------------------------Definition de fonctions ------------------------
Fonction du test moteur vibrant
Sortie : vibration
-----------------------------------------------------------------------------*/
void testMoteurVibrant(){
  digitalWrite(moteur_vibration, HIGH); 
    delay(1000); 
    digitalWrite(moteur_vibration, LOW); 
    delay(1000);
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