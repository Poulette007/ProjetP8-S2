#pragma once
#include <string>
class Player
{
public:
	Player(std::string name, int score);
private:
	std::string name;
	int score;
};

