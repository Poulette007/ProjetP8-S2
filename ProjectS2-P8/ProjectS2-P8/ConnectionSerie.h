#pragma once
#include "include/serial/SerialPort.hpp"
#include "include/json.hpp"
#include <string>
#include <iostream>

#define BAUD 115200           // Frequence de transmission serielle
#define MSG_MAX_SIZE 1024   // Longueur maximale d'un messag

using json = nlohmann::json;
using namespace std;

class ConnectionSerie
{
private:
	
	std::string com = "COM7";
	static string raw_msg;
	static json json_data;
public:
	static SerialPort* SP;
	ConnectionSerie();
	static bool Send(SerialPort* SP, json j_msg);
	static bool Envoie(json j_msg);
	static bool ReceiveRaw(SerialPort* SP, std::string& msg);
	static bool isConnected();

	static int getValue(const string index);
	json getJson();
	static bool hasData();
};

