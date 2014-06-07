#include "sfml\Network\TcpListener.hpp"

sf::TcpListener listener; // tworzymy gniazdo nas³uchujace
unsigned int port = 54000; // port, na którym bêdziemy nas³uchiwaæ

if (listener.listen(port) != sf::Socket::Done) // rozpoczynamy nas³uchiwanie na porcie 'port'
{
	cerr << "Nie mogê rozpocz¹æ nas³uchiwania na porcie " << port << endl;
	exit(1);
}
while ( /*...*/)
{
	sf::TcpSocket client; // tworzymy gniazdo, dziêki któremu bêdziemy mogli siê komunikowaæ z klientem
	listener.accept(client);

	// wysy³anie/odbieranie danych od/do klienta
}