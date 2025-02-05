#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class EcranPrincipal {
public:
    void printMenu();
    void processChoice(int choix);

private:
    void logIn();
    void createAccount();
    bool userExists(const std::string& userName);
    void addUser(const std::string& userName);
    void contactUs();
};

