#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class EcranPrincipal {
public:
    void printMenu();
    void processChoice(int choix);

    std::string getUserName() const;
    int getScore() const;
private:
	std::string userName;
	int score;
    void logIn();
    void createAccount();
    bool userExists(const std::string& userName);
    void addUser(const std::string& userName);
    void contactUs();
};

