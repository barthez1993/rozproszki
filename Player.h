#pragma once
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace std;
template <class T>
class Counter
{
private:
	static int count;
public:
	Counter()
	{
		count++;
	}
	Counter(const Counter &c)
	{
		count++;
	}
	~Counter()
	{
		count--;
	}
	static int GetCount() {

		return count;
	}
};

template <class T>
int Counter<T>::count = 0;

class Player : private Counter<Player>
{
private:
	string name;
	int index;		//1, 2 3 lub 4
	int x;
	int y;
	int score;
	int lives;
	bool orientation;		//true pozioma, false pionowa
	sf::Color color;

	Player() {};	//prywatny domyœlny konstruktor
public:
	using Counter<Player>::GetCount;
	Player(string imie);	// konstruktor
	//getery
	bool getOrientation() { return orientation; }
	int getIndex() { return index; }
	string getName() { return name; }
	int getScore() { return score; }
	int getLives() { return lives; }
	int getX() { return x; }
	int getY() { return y; }
	sf::Color getColor() { return color; }
	string getStringScore()
	{
		ostringstream ss;
		ss << score;
		return ss.str();
	}
	//setery
	void setX(float dx){ x += dx; }
};