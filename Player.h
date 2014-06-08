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
	float x;
	float y;
	float width;
	int score;
	int lives;
	bool orientation;		//true pozioma, false pionowa
	sf::Color color;

public:
	using Counter<Player>::GetCount;
	Player();	// konstruktor
	//getery
	bool getOrientation() { return orientation; }
	int getIndex() { return index; }
	string getName() { return name; }
	int getScore() { return score; }
	int getLives() { return lives; }
	float getX() { return x; }
	float getY() { return y; }
	float getWidth() { return width; }
	sf::Color getColor() { return color; }
	string getStringScore()
	{
		ostringstream ss;
		ss << score;
		return ss.str();
	}
	//setery
	void setX(float dx){ x += dx; }
	//klawiatura
	void moveLeft()
	{ 
		if (getOrientation())
			x = x - 5;
		else
			y = y - 5;
	}
	void moveRight() 
	{ 
		if (getOrientation())
			x = x + 5; 
		else
			y = y + 5;
	}

	void setName( std::string playerName )
	{
		name = playerName;
	}

	void setPosition(float pozycjaX, float pozycjaY)
	{
		x = pozycjaX;
		y = pozycjaY;
	}
};