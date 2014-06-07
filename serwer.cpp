#include "sfml\Network\TcpListener.hpp"

sf::TcpListener listener; // tworzymy gniazdo nas�uchujace
unsigned int port = 54000; // port, na kt�rym b�dziemy nas�uchiwa�

if (listener.listen(port) != sf::Socket::Done) // rozpoczynamy nas�uchiwanie na porcie 'port'
{
	cerr << "Nie mog� rozpocz�� nas�uchiwania na porcie " << port << endl;
	exit(1);
}
while ( /*...*/)
{
	sf::TcpSocket client; // tworzymy gniazdo, dzi�ki kt�remu b�dziemy mogli si� komunikowa� z klientem
	listener.accept(client);

	// wysy�anie/odbieranie danych od/do klienta
}