#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <conio.h>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include "Player.h"
#include "Serwer.h"
#include "Klient.h"
#include "Ball.h"
Player gracze[4];

using namespace std;

//zmienne globalne rz�dz�!
sf::ContextSettings settings(0, 0, 4, 2, 1);	//wygladzanie krawedzi x4
sf::RenderWindow window(sf::VideoMode(1280, 720), "ARKANOID", sf::Style::Default, settings);
sf::Clock timer;
Serwer serwer;
Ball ball;
Klient klient;
bool czySerwer;
bool oknoAktywne;//czy okno jest aktywne
bool gameStarted=false;
string **mapa;

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
	name.setString(player.getName() + "\n" + player.getStringScore());
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
		heartSprite.move(1280 - i * 80, y + 10);
		window.draw(heartSprite);
	}
	createPlayerPaddle(player);
}

void przesunPaletkiGraczyNaSerwerze()
{
	//Przechodzimy po wszystkich graczach przechowywanych w std map
	for (Serwer::Gracze::iterator i = serwer.gracze.begin(); i != serwer.gracze.end(); i++)
	{
		DaneGracza daneGracza = i->second;
		switch (daneGracza.numerGracza)
		{
		case 1:
			if (daneGracza.kierunekPaletki == -1)
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
		dane << gracze[i].getScore();
		dane << gracze[i].getLives();
	}
	dane << ball.getX();
	dane << ball.getY();
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			dane << mapa[i][j];
		}
	}
	
	serwer.wyslijPakietDoGraczy(dane);//iterujemy po wektorze z klientami
	//i dla kazdego z nich wysylamy dane
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
			gracze[0].setScore(klient.odebraneDane.wynik[i]);
			gracze[0].setLives(klient.odebraneDane.zycia[i]);
			break;
		case 1:
			gracze[1].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			gracze[1].setScore(klient.odebraneDane.wynik[i]);
			gracze[1].setLives(klient.odebraneDane.zycia[i]);
			break;
		case 2:
			gracze[2].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			gracze[2].setScore(klient.odebraneDane.wynik[i]);
			gracze[2].setLives(klient.odebraneDane.zycia[i]);
			break;
		case 3:
			gracze[3].setPosition(klient.odebraneDane.pozycjaX[i], klient.odebraneDane.pozycjaY[i]);
			gracze[3].setScore(klient.odebraneDane.wynik[i]);
			gracze[3].setLives(klient.odebraneDane.zycia[i]);
			break;
		}
	}
	ball.setPosition(klient.odebraneDane.pilkaX, klient.odebraneDane.pilkaY);
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 12; j++)
		{
			mapa[i][j] = klient.odebraneDane.mapa[i][j];
		}
	}
}
void wczytaj_mape()	//wczytuje do tablicy charow[12][12]
{
	int x = 0;
	int y = 0;


	fstream plik;
	plik.open("res/mapa.txt", ios::in);
	if (plik.good())
	{
		while (!plik.eof())
		{
			for (x = 0; x<12; x++)
			{
				for (y = 0; y<12; y++)
				{
					plik >> mapa[x][y];
				}
			}
		}
		plik.close();
	}

}

void createMap()
{

	int kolumna = 0;
	int wiersz = 0;
	float x = 180, y = 180;
	sf::RectangleShape playWindow;

	for (wiersz = 0; wiersz < 12; wiersz++)
	{
		for (kolumna = 0; kolumna < 12; kolumna++)
		{
			if (mapa[wiersz][kolumna] == "R")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Red);
				window.draw(playWindow);
			}
			if (mapa[wiersz][kolumna] == "G")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Green);
				window.draw(playWindow);
			}
			if (mapa[wiersz][kolumna] == "B")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Blue);
				window.draw(playWindow);
			}
			if (mapa[wiersz][kolumna] == "Y")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Yellow);
				window.draw(playWindow);
			}
			if (mapa[wiersz][kolumna] == "M")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Magenta);
				window.draw(playWindow);
			}
			if (mapa[wiersz][kolumna] == "C")
			{
				playWindow.setPosition(x, y);
				playWindow.setSize(sf::Vector2f(28, 28));
				playWindow.setFillColor(sf::Color::Cyan);
				window.draw(playWindow);
			}

			x += 30;

		}
		y += 30;
		x = 180;
	}

}

void createBall(Ball ball)
{
	sf::CircleShape ballShape(5);
	ballShape.setFillColor(sf::Color::White);
	ballShape.setPosition(ball.getX(), ball.getY());
	window.draw(ballShape);
}

int main(int argc, char *argv[])
{

	gracze[0].setName("Gracz 1");
	gracze[1].setName("Gracz 2");
	gracze[2].setName("Gracz 3");
	gracze[3].setName("Gracz 4");
	czySerwer = true;
	oknoAktywne = true;

	mapa = new string *[12];
	for (int i = 0; i<12; i++)
		mapa[i] = new string[12];

	if (czySerwer)
	{

		bool czyUruchomiono = serwer.uruchomSerwer();
		if (!czyUruchomiono)
			czySerwer = false;

	}

	wczytaj_mape();
	//klient.podlaczDoSerwera(argv[1]);
	klient.podlaczDoSerwera("127.0.0.1");
	//klient.podlaczDoSerwera("192.168.0.12");
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
			/*if (serwer.dajLiczbeGraczy() == 4 && gameStarted==false)
			{
				ball.setDy(1);
				gameStarted = true;
			}*/
		}

		//Obs�uga komunikacji klienta
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

		// clear the window with black color
		window.clear(sf::Color::Black);

		// draw everything here...
		// window.draw(...);
		

		createMap();

		//rysowanie okien gracza
		//rysowanie okien gracza
		for (int i = 0; i <4; i++)
			createPlayerWindow(gracze[i]);
		createBall(ball);
		if (czySerwer && serwer.dajLiczbeGraczy() >= 4)
			ball.update();
		int mapaX;
		int mapaY;
		
		/* do debuga, pozniej wykomentowac */
		if (gameStarted == false)
		{
			ball.setDx(0);
			ball.setDy(1);
			//ball.setPosition(150, 360);
			gameStarted = true;
		}
		//czy jest w srodkowym kwadracie
		//wszedzie +5 bo obliczamy dla �rodka pilki a nie jej gornego lewego rogu
		if (ball.getX()+5 >= 180 && ball.getX()+5 <= 540 && ball.getY()+5 >= 180 && ball.getY()+5 < 540)
		{
			mapaX = (ball.getX()+5 - 180) / 30;
			mapaY = (ball.getY()+5 - 180) / 30;
			if (mapa[mapaY][mapaX] != " ")
			{
				mapa[mapaY][mapaX] = " ";
				if (ball.getLastPlayer() == 2 || ball.getLastPlayer() == 3)
					ball.setDx(ball.getDx()*-1);
				else if (ball.getLastPlayer() == 0 || ball.getLastPlayer() == 0)
					ball.setDy(ball.getDy()*-1);
				gracze[ball.getLastPlayer()].addPoints(100);
			}	
		}
		
		//sprawdzanie kolizji z klockiem dla gracza 1
		//sprawdzanie kolizji z graczem
		else if (ball.getY()+10 == gracze[0].getY() && ball.getX()+10>=gracze[0].getX() && ball.getX()+10 <= gracze[0].getX()+gracze[0].getWidth())		//gracz 1
		{
			float pointB = ball.getX();			// wspolrzedna X kolizji pilki
			float pointP = gracze[0].getX();	//wspolrzedna X kolizji paletki
			float pointW = gracze[0].getWidth() / 2;
			float angleX = (pointB - pointP - pointW) / pointW;
			ball.setDx(angleX);
			ball.setDy(-1);
			ball.setLastPlayer(0);
		}
		else if (ball.getY() - 10 == gracze[1].getY() && ball.getX() - 10 >= gracze[1].getX() && ball.getX() - 10 <= gracze[1].getX() + gracze[1].getWidth())		//gracz 2
		{
			float pointB = ball.getX();			// wspolrzedna X kolizji pilki
			float pointP = gracze[1].getX();	//wspolrzedna X kolizji paletki
			float pointW = gracze[1].getWidth() / 2;
			float angleX = (pointB - pointP - pointW) / pointW;
			ball.setDx(angleX);
			ball.setDy(1);
			ball.setLastPlayer(1);
		}
		else if (ball.getX() - 10 == gracze[2].getX() && ball.getY() - 10 >= gracze[2].getY() && ball.getY() - 10 <= gracze[2].getY() + gracze[2].getWidth())		//gracz 3
		{
			float pointB = ball.getY();			// wspolrzedna Y kolizji pilki
			float pointP = gracze[2].getY();	//wspolrzedna Y kolizji paletki
			float pointW = gracze[2].getWidth() / 2;
			float angleY = (pointB - pointP - pointW) / pointW;
			ball.setDx(1);
			ball.setDy(angleY);
			ball.setLastPlayer(2);
		}
		else if (ball.getX() + 10 == gracze[2].getX() && ball.getY() + 10 >= gracze[3].getY() && ball.getY() + 10 <= gracze[3].getY() + gracze[3].getWidth())		//gracz 4
		{
			float pointB = ball.getY();			// wspolrzedna Y kolizji pilki
			float pointP = gracze[3].getY();	//wspolrzedna Y kolizji paletki
			float pointW = gracze[3].getWidth() / 2;
			float angleY = (pointB - pointP - pointW) / pointW;
			ball.setDx(-1);
			ball.setDy(angleY);
			ball.setLastPlayer(3);
		}
		//wyjscie pilki poza zakres
		if (ball.getX() <= 0)
		{
			gracze[2].removeLive();
			ball.setPosition(150, 360);
			ball.setDy(0);
			ball.setDx(-1);
		}
		if (ball.getX() >= 730)
		{
			gracze[3].removeLive();
			ball.setPosition(560, 360);
			ball.setDy(0);
			ball.setDx(1);
		}
		if (ball.getY() <= 0)
		{
			gracze[1].removeLive();
			ball.setPosition(360, 150);
			ball.setDy(-1);
			ball.setDx(0);
		}
		if (ball.getY() >= 730)
		{
			gracze[0].removeLive();
			ball.setPosition(360, 560);
			ball.setDy(1);
			ball.setDx(0);
		}
		window.display();
	}
	klient.rozlaczKlienta();
	serwer.zatrzymajSerwer();

	return 0;
}