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
	string raw_msg;
	json json_data;
public:
	SerialPort* SP;
	ConnectionSerie();
	bool Send(SerialPort* SP, json j_msg);
	bool ReceiveRaw(SerialPort* SP, std::string& msg);
	bool isConnected();

	int getValue(const string index);
	json getJson();
	bool hasData();
};

