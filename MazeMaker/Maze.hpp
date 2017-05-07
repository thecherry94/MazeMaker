#pragma once

#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Graphics.hpp>

#include <memory>
#include <random>

#include "MazeCell.hpp"

class Maze : public sf::Drawable, public sf::Transformable
{
	private:
		std::vector<std::shared_ptr<MazeCell>> _cells;
		sf::Vector2u _size;
		sf::Vector2u _cell_size;
		std::vector<sf::VertexArray> _render_vertices;
		sf::RenderTexture _tex;

		float _block_width; 
		float _block_height;
		
		sf::RenderWindow* _p_win;

		bool _needs_redraw;

	public:
		Maze();
		Maze(sf::RenderWindow* p_win, sf::Vector2u size);

		void render(sf::RenderTarget& target);
		void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		void setScale(float fx, float fy)
		{
			sf::Transformable::setScale(fx, fy);

			int width = _size.x;
			int height = _size.y;

			int win_width = _p_win->getSize().x;
			int win_height = _p_win->getSize().y;

			_tex.create(width * (_block_width + 1) * getScale().x, height * (_block_height + 1) * getScale().y + 10);

			for (int i = 0; i < _size.x * _size.y; i++)
				_cells[i]->setScale(fy, fy);

			_needs_redraw = true;
		}


		void setPosition(sf::Vector2f pos)
		{
			//for (int i = 0; i < _size.x * _size.y; i++)
			//	_cells[i]->setPosition(pos);



			//_needs_redraw = true;

			sf::Transformable::setPosition(pos);
		}

		void make_maze_random_walk(sf::Vector2u start, sf::Vector2u goal);
		void make_maze_random_walk();
		void reset();


};