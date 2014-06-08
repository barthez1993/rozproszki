#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <Windows.h>
#include "Player.h"
#include "Serwer.h"
#include "Klient.h"

Player gracze[4];


//zmienne globalne rz¹dz¹!
sf::ContextSettings settings(0,0,4,2,1);	//wygladzanie krawedzi x4
sf::RenderWindow window(sf::VideoMode(1280, 720), "ARKANOID", sf::Style::Default, settings);
sf::Clock timer;
Serwer serwer;
Klient klient;
bool czySerwer;
bool oknoAktywne;//czy okno jest aktywne


void createPlayerPaddle(Player player)		//tworzenie paletki
{
	sf::RectangleShape paddle;
	if (player.getOrientation()) paddle.setSize(sf::Vector2f(player.getWidth(), 15));
	else paddle.setSize(sf::Vector2f(15, player.getWidth()));
	paddle.setFillColor(player.getColor());
	
	//if (player.getOrientation()) player.setX(timer.getElapsedTime().asSeconds()*60);	//do debuga
	paddle.move(player.getX(), player.getY());
	window.draw(paddle);
}

void createPlayerWindow(Player player)
{
	sf::RectangleShape playerWindow(sf::Vector2f(560, 180));
	playerWindow.setFillColor(sf::Color());
	playerWindow.setOutlineThickness(-5);
	int y = (player.getIndex() - 1) * 180;
	playerWindow.setOutlineColor(player.getColor());
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
	name.setColor(player.getColor());
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
		heartSprite.setColor(player.getColor());
		heartSprite.move(1280-i*80, y+10);
		window.draw(heartSprite);
	}
	createPlayerPaddle(player);
}


void przesunPaletkiGraczyNaSerwerze()
{
	//Przechodzimy po wszystkich graczach przechowywanych w mapie
	for (Serwer::Gracze::iterator i = serwer.gracze.begin(); i != serwer.gracze.end(); i++)
	{
		DaneGracza daneGracza = i->second;
		switch (daneGracza.numerGracza)
		{
		case 1:
			if (daneGracza.kierunekPaletki==-1)
				gracze[0].moveLeft();

			if (daneGracza.kierunekPaletki == 1)
				gracze[0].moveRight();

			break;
		case 2:
			if (daneGracza.kierunekPaletki == -1)
				gracze[1].moveLeft();

			if (daneGracza.kierunekPaletki == 1)
				gracze[1].moveRight();
			break;
		case 3:
			if (daneGracza.kierunekPaletki == -1)
				gracze[2].moveLeft();

			if (daneGracza.kierunekPaletki == 1)
				gracze[2].moveRight();
			break;
		case 4:
			if (daneGracza.kierunekPaletki == -1)
				gracze[3].moveLeft();

			if (daneGracza.kierunekPaletki == 1)
				gracze[3].moveRight();
			break;
		}
	}
}

void wyslijDaneDoKlientow()
{
	sf::Packet dane;
	for (int i = 0; i < 4; i++)
	{
		dane << i;
		dane << gracze[i].getX();
		dane << gracze[i].getY();
	}
	serwer.wyslijPakietDoGraczy(dane);
}

void wyslijDaneDoSerwera()
{
	sf::Packet dane;
	sf::Int32 kierunek = 0;
	if (oknoAktywne && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		kierunek = -1;
	}
	else if (oknoAktywne && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		kierunek = 1;
	}
	dane << kierunek;
	klient.wyslijPakietDoSerwera(dane);
}

void aktualizujPozycjeGraczyNaKliencie()
{
	for (int i = 0; i < 4; i++)
	{
		switch (klient.odebraneDane.idGracza[i])
		{
		case 0:
			gracze[0].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			break;
		case 1:
			gracze[1].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			break;
		case 2:
			gracze[2].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			break;
		case 3:
			gracze[3].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			break;
		}
	}
}
// WYSLANEEEEEE
int main()
{
	gracze[0].setName("Gracz 1");
	gracze[1].setName("Gracz 2");
	gracze[2].setName("Gracz 3");
	gracze[3].setName("Gracz 4");
	czySerwer = true;
	oknoAktywne = true;

	if (czySerwer)
	{
		bool czyUruchomiono = serwer.uruchomSerwer();
		if (!czyUruchomiono)
			czySerwer = false;

	}
	

	klient.podlaczDoSerwera("127.0.0.1");
	// run the program as long as the window is open
	while (window.isOpen())
	{
		// check all the window's events that were triggered since the last iteration of the loop
		sf::Event event;
		while (window.pollEvent(event))
		{
			// "close requested" event: we close the window
			switch (event.type)
			{
			case sf::Event::Closed:
				window.close();
				break;

			case sf::Event::LostFocus:
				oknoAktywne = false;
				break;

			case sf::Event::GainedFocus:
				oknoAktywne = true;
			}		
		}

		//Obsluga komunikacji serwera
		if (czySerwer)
		{
			serwer.oczekujNaNowegoKlienta();
			serwer.odbierzDaneOdKlientow();
			wyslijDaneDoKlientow();
			przesunPaletkiGraczyNaSerwerze();
		}

		//Obs³uga komunikacji klienta
		{
			wyslijDaneDoSerwera();
			klient.odbierzDaneOdSerwera();
			if (!czySerwer)
				aktualizujPozycjeGraczyNaKliencie();
		}


		// obsluga wyjscia z aplikacji za pomoca ESC
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
		{
			window.close();
		}


		//costam
		sf::RectangleShape gameField(sf::Vector2f(720, 720));	//wymiary pola gry, musi byc kwadrat
		gameField.setFillColor(sf::Color());		//kolor RGB do wypelnienia

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);

		//rysowanie okien gracza
		for (int i = 0; i <4;i++)
		createPlayerWindow(gracze[i]);

		//rysowanie reszty
		//window.draw(gameField);

		// end the current frame
		window.display();
	}
	klient.rozlaczKlienta();
	serwer.zatrzymajSerwer();

	return 0;
}