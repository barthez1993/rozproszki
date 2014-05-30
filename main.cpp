#include <SFML/Graphics.hpp>
#include <cmath>
using namespace sf;
int main()
{
	RenderWindow okno(VideoMode(1024, 600), "Kurs SFML 2.0 - http://cpp0x.pl");
	Clock stoper;
	while (okno.isOpen())
	{
		Event event;
		while (okno.pollEvent(event))
		{
			if (event.type == Event::Closed)
				okno.close();

		} //while
		okno.clear();

		CircleShape ksztalt(std::sin(stoper.getElapsedTime().asSeconds()) * okno.getSize().y / 8 + okno.getSize().y / 4);
		ksztalt.setOrigin(Vector2f(ksztalt.getRadius(), ksztalt.getRadius()));
		ksztalt.setPosition(okno.getSize().x / 2.0f, okno.getSize().y / 2.0f);
		ksztalt.setFillColor(Color::Yellow);
		okno.draw(ksztalt);

		okno.display();
	} //while
	return 0;
}