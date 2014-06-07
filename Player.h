#pragma once
#include <string>
#include <sstream>
#include <SFML/Graphics.hpp>

using namespace std;
class Player
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

	Player() {};	//prywatny domyœlny konstruktor
public:
	Player(string imie, int numer);	// konstruktor
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
	void moveLeft() { x-=5; }
	void moveRight() { x+=5; }
};