#include <SFML/Network.hpp>
#include <SFML/System/Clock.hpp>

struct DaneOdSerwera {
	int idGracza[4];
	float pozycjaX[4];
	float pozycjaY[4];
	int wynik[4];
	int zycia[4];
	float pilkaX;
	float pilkaY;
	std::string mapa[12][12];
};

class Klient {
public:

	sf::TcpSocket socket;
	DaneOdSerwera odebraneDane;
	sf::Clock zegar;

	
	bool podlaczDoSerwera(std::string adres, int port = 12345){

		sf::Socket::Status status = socket.connect(adres, port);
		if (status != sf::Socket::Done)//jesli blad
		{
			return false;
		}
		socket.setBlocking(false);
		return true;
	}
	void wyslijPakietDoSerwera(sf::Packet dane){
		if (zegar.getElapsedTime().asMilliseconds() < 100)
			return;//nie wysy³amy danych, je¿eli nie up³yn¹³ okreœlony czas

		//restartujemy czas
		zegar.restart();

		//wysy³amy dane

		socket.send(dane);
	}
	/*void odbierzDaneOdSerwera(){
		sf::Packet dane;
		if (socket.receive(dane) == sf::Socket::Done){//dane udalo sie odebrac
			for (int i = 0; i < 4; i++){
				dane >> odebraneDane.idGracza[i];
				dane >> odebraneDane.pozycjaX[i];
				dane >> odebraneDane.pozycjaY[i];
			}
		}
	}*/

	void odbierzDaneOdSerwera(){
		sf::Packet dane;
		if (socket.receive(dane) == sf::Socket::Done){//dane udalo sie odebrac
			for (int i = 0; i < 4; i++){
				dane >> odebraneDane.idGracza[i];
				dane >> odebraneDane.pozycjaX[i];
				dane >> odebraneDane.pozycjaY[i];
				dane >> odebraneDane.wynik[i];
				dane >> odebraneDane.zycia[i];
			}
			dane >> odebraneDane.pilkaX;
			dane >> odebraneDane.pilkaY;
			for (int i = 0; i < 12; i++)
			{
				for (int j = 0; j < 12; j++)
				{
					dane >> odebraneDane.mapa[i][j];
				}
			}
		}
	}
	void rozlaczKlienta()
	{
		socket.disconnect();
	}
};