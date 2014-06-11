#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>
#include <vector>
#include <map>

struct DaneGracza
{
	//sf::String nick;
	sf::Int32 kierunekPaletki;
	int numerGracza;
};

class Serwer
{
public:
	sf::TcpListener listener;//utworzenie serwera nas³uchuj¹cego 
	sf::SocketSelector selector; // do obs³ugi wielu socketów
	std::vector<sf::TcpSocket*> clients;
	sf::Clock zegar;//Zegar do ograniczania czêstotliwoœci wysy³anych pakietów


	typedef std::map<std::string, DaneGracza> Gracze;
	Gracze gracze;

	int dajLiczbeGraczy()
	{
		return clients.size();
	}

	bool uruchomSerwer()
	{
		// bind the listener to a port
		if (listener.listen(12345) != sf::Socket::Done)
		{
			return false;
		}

		listener.setBlocking(false);
		selector.add(listener);
		return true;
	}

	void oczekujNaNowegoKlienta()
	{
		//http://www.sfml-dev.org/documentation/2.0/classsf_1_1SocketSelector.php

		if (selector.wait(sf::milliseconds(1)))
		{
			// Test the listener
			if (selector.isReady(listener))
			{
				// The listener is ready: there is a pending connection
				sf::TcpSocket* nowySocket = new sf::TcpSocket;
				if (listener.accept(*nowySocket) == sf::Socket::Done)
				{
					// Add the new client to the clients list
					clients.push_back(nowySocket);
					// Add the new client to the selector so that we will
					// be notified when he sends something
					selector.add(*nowySocket);
				}
				else
				{
					// Error, we won't get a new connection, delete the socket
					delete nowySocket;
				}
			}
		}
	}

	void odbierzDaneOdKlientow()
	{
		//http://www.sfml-dev.org/documentation/2.0/classsf_1_1SocketSelector.php

		if (selector.wait(sf::milliseconds(1)))
		{
			// Test the listener
			if (!selector.isReady(listener))
			{
				// The listener socket is not ready, test all other sockets (the clients)
				for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
				{
					sf::TcpSocket* socketKlienta = *it;//odczytuje wskaznik do socketa
					if (selector.isReady(*socketKlienta))//czy socket jest gotowy do odebrania danych
					{
						// The client has sent some data, we can receive it
						sf::Packet odebranyPakiet;
						//odbieram dane
						if (socketKlienta->receive(odebranyPakiet) == sf::Socket::Done)
						{//jesli dane udalo sie odebrac to przetwarzamy je
							obsluzDaneKlienta(*socketKlienta, odebranyPakiet);
						}
					}
				}
			}
		}
	}

	void obsluzDaneKlienta(sf::TcpSocket& socketKlienta, sf::Packet& dane)
	{
		//Budujemy unikatowy identyfikator gracza w postaci: ADRES_IP:PORT
		std::string adresGracza = socketKlienta.getRemoteAddress().toString();
		adresGracza = adresGracza + ":";
		adresGracza = adresGracza + std::to_string(socketKlienta.getRemotePort());

		int odczytanyKierunek;
		dane >> odczytanyKierunek;//odczytanie z pakietu do zmiennej
		
		if (gracze.count(adresGracza) == 0)
		{
			// nie ma gracza jeszcze w mapie, trzeba nadaæ mu jakiœ numer
			int nowyNumerGracza = gracze.size() + 1;
			gracze[adresGracza].numerGracza = nowyNumerGracza;
		}//Zapisujemy dane do mapy
		gracze[adresGracza].kierunekPaletki = odczytanyKierunek;
	}

	void zatrzymajSerwer()
	{
		listener.close();
	}

	void wyslijPakietDoGraczy(sf::Packet dane)
	{
		if (zegar.getElapsedTime().asMilliseconds() < 100)
			return;//nie wysy³amy danych, je¿eli nie up³yn¹³ okreœlony czas

		//restartujemy czas
		zegar.restart();

		//wysy³amy dane
		for (std::vector<sf::TcpSocket*>::iterator it = clients.begin(); it != clients.end(); ++it)
		{//do kazdego socketa wysylam dane
			sf::TcpSocket* socketKlienta = *it;
			socketKlienta->send(dane);
		}

	}

};