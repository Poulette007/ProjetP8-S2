/*
* Author: Manash Kumar Mandal
* Modified Library introduced in Arduino Playground which does not work
* This works perfectly
* LICENSE: MIT
*/

#pragma once

#define ARDUINO_WAIT_TIME 2000
#define MAX_DATA_LENGTH 255

#include <windows.h>
#include <iostream>

class SerialPort
{
private:
    
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    explicit SerialPort(const char *portName, int BAUD);
    ~SerialPort();
    HANDLE handler;
    int readSerialPort(const char *buffer, unsigned int buf_size);
    bool writeSerialPort(const char *buffer, unsigned int buf_size);
    bool isConnected();
    void closeSerial();
	HANDLE getHandler() const;
	DWORD getErrors() const;
	COMSTAT getStatus() const;
};