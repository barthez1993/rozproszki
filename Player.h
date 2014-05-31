#pragma once
#include <string>
using namespace std;
class Player
{
public:
	Player();
	Player(string imie, int numer)
	{
		name = imie;
		index = numer;
		score = 0;
		//if index = 1 to x =... y=... dla wszystkich pozycje startowe
	}
	~Player();
private:
	string name;
	int index;		//1, 2 3 lub 4
	int x;
	int y;
	int score;

};

