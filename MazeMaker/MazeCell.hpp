#pragma once

#include <SFML\Graphics.hpp>
#include <SFML\Window.hpp>
#include <SFML\System.hpp>

enum EMazeDirection
{
	TOP,
	RIGHT,
	BOTTOM,
	LEFT
};

class MazeCell : public sf::Drawable, public sf::Transformable
{
	private:
		sf::Vector2u _pos;
		sf::Vector2f _size;
		bool _walls[4];					// true = wall, false = no wall
		sf::VertexArray _vertices[4];
		bool _needs_redraw;

	public:
		MazeCell(sf::Vector2u pos, sf::Vector2f size)
			: _pos(pos), _size(size)
		{
			const sf::Color color = sf::Color::Black;

			_walls[0] = true;
			_walls[1] = true;
			_walls[2] = true;
			_walls[3] = true;

			float x = _pos.x;
			float y = _pos.y;

			float width = _size.x;
			float height = _size.y;

			for (int i = 0; i < 4; i++)
			{
				_vertices[i].setPrimitiveType(sf::Lines);
				_vertices[i].resize(2);
			}

			float x_off = 3;
			float y_off = 3;

			// TOP
			_vertices[0][0].position = sf::Vector2f(x * width + x_off, y * height + y_off);
			_vertices[0][1].position = sf::Vector2f((x + 1) * width + x_off, y * height + y_off);
			_vertices[0][0].color = color;
			_vertices[0][1].color = color;

			// RIGHT
			_vertices[1][0].position = sf::Vector2f((x + 1) * width + x_off, y * height + y_off);
			_vertices[1][1].position = sf::Vector2f((x + 1) * width + x_off, (y + 1) * height + y_off);
			_vertices[1][0].color = color;
			_vertices[1][1].color = color;

			// BOTTOM
			_vertices[2][0].position = sf::Vector2f((x + 1) * width + x_off, (y + 1) * height + y_off);
			_vertices[2][1].position = sf::Vector2f(x * width + x_off, (y + 1) * height + y_off);
			_vertices[2][0].color = color;
			_vertices[2][1].color = color;

			// LEFT
			_vertices[3][0].position = sf::Vector2f(x * width + x_off, y * height + y_off);
			_vertices[3][1].position = sf::Vector2f(x * width + x_off, (y + 1) * height + y_off);
			_vertices[3][0].color = color;
			_vertices[3][1].color = color;
		}

		void draw(sf::RenderTarget& target, sf::RenderStates states) const
		{
			states.transform *= getTransform();
			for (int i = 0; i < 4; i++)
			{
				// If not a wall, don't render wall
				if (!_walls[i])
					continue;

				target.draw(_vertices[i]);
			}	
		}

		void setScale(float fx, float fy)
		{
			sf::Transformable::setScale(fx, fy);

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 2; j++)
				{
					_vertices[i][j].position.x *= fx;
					_vertices[i][j].position.y *= fy;
				}
		}

		void setPosition(sf::Vector2f pos)
		{
			sf::Vector2f old_pos = getPosition();

			float x = pos.x;
			float y = pos.y;

			for (int i = 0; i < 4; i++)
				for (int j = 0; j < 2; j++)
				{
					_vertices[i][j].position.x += x;
					_vertices[i][j].position.y += y;
				}

			
			sf::Transformable::setPosition(_vertices[0][0].position);
		}

		sf::Rect<float> get_screen_bounds()
		{
			sf::Rect<float> r;
			sf::Vector2f p = (sf::Vector2f)_pos;
			r.left = (p.x * _size.x);
			r.top = (p.y * _size.y);
			
			r.width = _size.x;
			r.height = _size.y;

			return r;
		}

		sf::Vector2u get_mace_pos()
		{
			return _pos;
		}

		void set_wall(EMazeDirection dir, bool closed)
		{
			_walls[(int)dir] = closed;
		}
};