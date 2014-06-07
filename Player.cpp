#include "Player.h"


Player::Player(string imie)
{
	width = 150;
	name = imie;
	index = GetCount();
	score = 0;
	lives = 3;
	switch (index)
	{
	case 1:
		y = 700;
		x = 285;
		orientation = true;
		color = sf::Color::Red;
		break;
	case 2:
		y = 5;
		x = 285;
		orientation = true;
		color = sf::Color::Green;
		break;
	case 3:
		y = 285;
		x = 5;
		orientation = false;
		color = sf::Color::Blue;
		break;
	case 4:
		y = 285;
		x = 700;
		orientation = false;
		color = sf::Color::Yellow;
		break;
	}
}

