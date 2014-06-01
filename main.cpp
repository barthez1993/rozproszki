#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Player.h"

Player pierwszy("Bartek", 1);
Player drugi("Maria", 2);
Player trzeci("Michal", 3);
Player czwarty("JanDaciuk", 4);

//zmienne globalne rz�dz�!
sf::ContextSettings settings(0,0,4,2,1);	//wygladzanie krawedzi x4
sf::RenderWindow window(sf::VideoMode(1280, 720), "ARKANOID", sf::Style::Default, settings);


void createPlayerWindow(Player player)
{
	sf::RectangleShape playerWindow(sf::Vector2f(560, 180));
	playerWindow.setFillColor(sf::Color());
	playerWindow.setOutlineThickness(-5);
	int y = (player.getIndex() - 1) * 180;
	sf::Color kolor;
	switch (player.getIndex())
	{
	case 1:		
		kolor = sf::Color::Red;	
		break;
	case 2:
		kolor = sf::Color::Green;
		break;
	case 3:
		kolor = sf::Color::Blue;
		break;
	case 4:
		kolor = sf::Color::Yellow;
		break;
	}
	playerWindow.setOutlineColor(kolor);
	sf::Font font;
	if (!font.loadFromFile("res/Roboto-Light.ttf"))
	{
		OutputDebugString("nie widzi czcionki!\n");
		/* print current directory
		TCHAR pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, pwd);
		MessageBox(NULL, pwd, pwd, 0); */
	}
	sf::Text name;
	name.setFont(font);
	name.setString(player.getName()+"\n"+player.getStringScore());
	name.setCharacterSize(60);
	name.setColor(kolor);
	playerWindow.move(720, y);
	name.move(720 + 20, y + 10);
	window.draw(playerWindow);
	window.draw(name);
	//zycia
	for (int i = 1; i <= player.getLives(); i++)
	{
		sf::Texture heart;
		heart.loadFromFile("res/heart.png");
		sf::Sprite heartSprite;
		heartSprite.setTexture(heart);
		heartSprite.setColor(kolor);
		heartSprite.move(1280-i*80, y+10);
		window.draw(heartSprite);
	}
}



// WYSLANEEEEEE
int main()
{
	// create the window



	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			if (event.type == sf::Event::Closed)
				window.close();
		}
		sf::RectangleShape gameField(sf::Vector2f(720, 720));	//wymiary pola gry, musi byc kwadrat
		gameField.setFillColor(sf::Color());		//kolor RGB do wypelnienia

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);

		//rysowanie okien gracza
		createPlayerWindow(pierwszy);
		createPlayerWindow(drugi);
		createPlayerWindow(trzeci);
		createPlayerWindow(czwarty);
		//rysowanie reszty
		//window.draw(gameField);

		// end the current frame
		window.display();
	}

	return 0;
}