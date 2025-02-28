#include "EcranPrincipal.h"
#include <iostream>

using namespace std;

void EcranPrincipal::printMenu() {
    cout << "================================" << endl;
    cout << "#### Bonjour ####" << endl;
    cout << "Choisir une option:" << endl;
    cout << "1. Connection" << endl;
    cout << "2. Creer un compte" << endl;
    cout << "3. Nous contacter" << endl;
    cout << "================================" << endl;
    cout << "Entrer votre choix: ";

    int choix;
    cin >> choix;
    processChoice(choix);
}

void EcranPrincipal::processChoice(int choix) {
    switch (choix) {
    case 1:
        logIn();
        break;
    case 2:
        createAccount();
        break;
    case 3:
        contactUs();
        break;
    default:
        cout << "Choix invalide. Veuillez réessayer." << endl;
        printMenu();
        break;
    }
}

std::string EcranPrincipal::getUserName() const
{
    return std::string();
}

int EcranPrincipal::getScore() const
{

    return this->score;
}

void EcranPrincipal::logIn() {
    cout << "Entrez votre nom d'utilisateur : ";
    string userName;
    cin >> userName;

    if (userExists(userName)) {
        cout << "Connected" << endl;
		this->userName = userName;
    }
    else {
        cout << "User not found. Please, create an account." << endl;
        printMenu();
    }
}

void EcranPrincipal::createAccount() {
    cout << "Create a new account." << endl;
    cout << "Enter your user name: ";
    string userName;
    cin >> userName;

    if (userExists(userName)) {
        cout << "User name already exist. Please, try a new one or connect you." << endl;
    }
    else {
        addUser(userName);
        cout << "User account successful." << endl;
    }
	logIn();
}

bool EcranPrincipal::userExists(const std::string& userName) {
    ifstream file("DataBase.csv");
    string line;
    while (getline(file, line)) {
        if (line == userName) {
            
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}


void EcranPrincipal::addUser(const std::string& userName) {
    ofstream file("DataBase.csv", ios::app);
    file << userName << endl;
    file.close();
}


void EcranPrincipal::contactUs() {
    cout << "Call 911" << endl;
    printMenu();
    
}
