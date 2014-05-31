#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Player.h"

Player pierwszy("Bartek", 1);
Player drugi("Maria", 2);
Player trzeci("Michal", 3);
Player czwarty("JanDaciuk", 4);

void createPlayerWindow(Player player)
{

}



// WYSLANEEEEEE
int main()
{
	// create the window
	sf::RenderWindow window(sf::VideoMode(1280, 720), "ARKANOID");
	sf::Font font;
	sf::Text text;
	text.setFont(font);
	if (!font.loadFromFile("res/basictitlefont.ttf"))
	{
		OutputDebugString("nie widzi czcionki!\n");
		/* print current directory 
		TCHAR pwd[MAX_PATH];
		GetCurrentDirectory(MAX_PATH, pwd);
		MessageBox(NULL, pwd, pwd, 0); */
	}
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
		text.setString("Arkanoid kurwa!");
		text.setCharacterSize(96);			//w pikselach
		text.setColor(sf::Color::Red);
		text.setPosition(500, 500);
		sf::RectangleShape gameField(sf::Vector2f(720, 720));	//wymiary pola gry, musi byc kwadrat
		gameField.setFillColor(sf::Color(150, 50, 250));		//kolor RGB do wypelnienia

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);


		window.draw(gameField);
		window.draw(text);

		// end the current frame
		window.display();
	}

	return 0;
}