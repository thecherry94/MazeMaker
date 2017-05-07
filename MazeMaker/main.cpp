#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

#include "Maze.hpp"

int main()
{
	sf::RenderWindow win(sf::VideoMode(1000, 1000), "Maze Maker");
	win.setFramerateLimit(144);

	Maze m(&win, sf::Vector2u(100, 100));
	m.setScale(1.0f, 1.0f);

	bool drag = false;

	sf::Vector2f delta_mouse;
	sf::Vector2f delta_sensitivity(0, 0);

	m.make_maze_random_walk();

	sf::View view(win.getView());
	view.zoom(1.0f);
	win.setView(view);

	while (win.isOpen())
	{
		sf::Event ev;

		sf::Vector2f mouse_start = (sf::Vector2f)sf::Mouse::getPosition(win);

		while(win.pollEvent(ev))
		{ 
			if (ev.type == sf::Event::Closed)
				win.close();

			if (ev.type == sf::Event::MouseButtonPressed)
			{
				if (ev.mouseButton.button == sf::Mouse::Left)
					drag = true;
			}
			else if (ev.type == sf::Event::MouseButtonReleased)
			{
				if (ev.mouseButton.button == sf::Mouse::Left)
					drag = false;
			}
			else if (ev.type == sf::Event::MouseWheelScrolled)
			{
				float delta = ev.mouseWheelScroll.delta;
				float scalar = 1/(pow(2, delta));

				view.zoom(scalar);
				win.setView(view);
			}

			if (ev.type == sf::Event::KeyPressed)
			{
				if (ev.key.code == sf::Keyboard::Space)
				{
					m.reset();
					m.make_maze_random_walk();
				}
			}
		}

		if (drag)
		{
			m.setPosition(m.getPosition() + delta_mouse);
		}

		win.clear(sf::Color::White);
		m.render(win);
		win.display();

		delta_mouse = (sf::Vector2f)sf::Mouse::getPosition(win) - mouse_start;

		if (abs(delta_mouse.x) < delta_sensitivity.x)
			delta_mouse.x = 0;

		if (abs(delta_mouse.y) < delta_sensitivity.y)
			delta_mouse.y = 0;
	}

	return 0;
}