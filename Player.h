#pragma once
#include <string>
#include <sstream>

using namespace std;
class Player
{
private:
	string name;
	int index;		//1, 2 3 lub 4
	int x;
	int y;
	int score;
	int lives;
	Player() {};	//prywatny domyœlny konstruktor
public:
	Player(string imie, int numer);	// konstruktor


	int getIndex() { return index; }
	string getName() { return name; }
	int getScore() { return score; }
	int getLives() { return lives; }
	string getStringScore()
	{
		ostringstream ss;
		ss << score;
		return ss.str();
	}
};