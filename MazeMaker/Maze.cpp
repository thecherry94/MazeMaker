#include "Maze.hpp"

std::vector<std::shared_ptr<MazeCell>> get_cell_neighbors(std::vector<std::shared_ptr<MazeCell>>& cells, sf::Vector2u pos, sf::Vector2u size);

Maze::Maze()
{

}



Maze::Maze(sf::RenderWindow* p_win, sf::Vector2u size)
	: _p_win(p_win), _size(size), _needs_redraw(true)
{
	int width = size.x;
	int height = size.y;

	int win_width = _p_win->getSize().x;
	int win_height = _p_win->getSize().y;

	_block_width = 30; //(float)win_width / (float)width;
	_block_height = 30; //(float)win_height / (float)height;

	_cells.reserve(width * height);
	
	if (!_tex.create(width * _block_width, height * _block_height))
	{

	}



	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			_cells.push_back(std::make_shared<MazeCell>(MazeCell(sf::Vector2u(x, y), sf::Vector2f(_block_width, _block_height))));
		}
	}
}


void Maze::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();

	int width = _size.x;
	int height = _size.y;

	int win_width = _p_win->getSize().x;
	int win_height = _p_win->getSize().y;

	int cells_x = (float)win_width / _block_width;
	int cells_y = (float)win_height / _block_height;

	int x_off = getPosition().x;
	int y_off = getPosition().y;

	sf::FloatRect win_rect(0, 0, win_width, win_height);

	// Only draw visible cells
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			std::shared_ptr<MazeCell> p_cell = _cells[y * width + x];
			//auto bounds = p_cell->get_screen_bounds();

			//if (!win_rect.intersects(bounds))
			//	continue;

			target.draw(*p_cell.get());
		}

	}
}



void Maze::render(sf::RenderTarget& target)
{	

	if (_needs_redraw)
	{
		_needs_redraw = false;
		_tex.clear(sf::Color::White);

		for (int i = 0; i < _size.x * _size.y; i++)
		{
			_tex.draw(*_cells[i]);
		}
		_tex.display();
	}

	sf::Sprite sprite(_tex.getTexture());
	sprite.setPosition(getPosition() + sf::Vector2f(1, 1));
	target.draw(sprite);
}


void Maze::make_maze_random_walk(sf::Vector2u start, sf::Vector2u goal)
{


	std::map<std::shared_ptr<MazeCell>, bool> visited;								// quick check if a cell has been visited already
	std::map<std::shared_ptr<MazeCell>, std::shared_ptr<MazeCell>> came_from;		// the path where the last tile in succession came from
	int univisited = _size.x * _size.y;

	std::shared_ptr<MazeCell> current = _cells[start.y * _size.y + start.x];
	current->set_wall(EMazeDirection::LEFT, false);

	visited[current] = true;
	univisited--;

	

	srand(time(NULL));

	while (univisited > 0)
	{

		// First, check if the current tile has any unvisitied neighbors
		// If there aren't any, walk back on cell
		//
		std::vector<std::shared_ptr<MazeCell>> neighbors; 
		bool has_unvisited_neighbors = false;
		do 
		{
			neighbors = get_cell_neighbors(_cells, current->get_mace_pos(), _size);
			std::vector<std::shared_ptr<MazeCell>>::iterator nb_it = neighbors.begin();

			for (nb_it; nb_it != neighbors.end(); nb_it++)
			{
				if (!visited[*nb_it])
				{
					has_unvisited_neighbors = true;
					break;
				}
			}
			
			if (!has_unvisited_neighbors)
			{
				current = came_from[current];
			}

		} while (!has_unvisited_neighbors);


		// Remove all neighbors that have been visited already
		//
		std::vector<std::shared_ptr<MazeCell>>::iterator nb_it = neighbors.begin();
		while (nb_it != neighbors.end())
		{
			if (visited[*nb_it])
				nb_it = neighbors.erase(nb_it);
			else
				nb_it++;
		}

		// Randomly choose one of the remaining neighbors
		//
		int max = neighbors.size();
		int idx = rand() % max;
		std::shared_ptr<MazeCell> nb = neighbors[idx];

		// dp = delta position
		sf::Vector2i dp = (sf::Vector2i)nb->get_mace_pos() - (sf::Vector2i)current->get_mace_pos();
		EMazeDirection dir_c, dir_nb;


		// **********************************************
		// * Depending on the dx and dy in position,	*
		// * find out the direction for both tiles		*
		// **********************************************


		if (dp.y < 0)
		{
			dir_c = EMazeDirection::TOP;
			dir_nb = EMazeDirection::BOTTOM;
		}
		else if (dp.y > 0)
		{
			dir_c = EMazeDirection::BOTTOM;
			dir_nb = EMazeDirection::TOP;
		}
		else if (dp.x < 0)
		{
			dir_c = EMazeDirection::LEFT;
			dir_nb = EMazeDirection::RIGHT;
		}
		else if (dp.x > 0)
		{
			dir_c = EMazeDirection::RIGHT;
			dir_nb = EMazeDirection::LEFT;
		}

		// Disable walls
		current->set_wall(dir_c, false);		
		nb->set_wall(dir_nb , false);

		// register the neighbor as visited
		visited[nb] = true;

		// add the current tile to the backtrack path
		came_from[nb] = current;

		// countdown the number of unvisited cells
		univisited--;

		// Move to the selected neighbor
		current = nb;
	}

	_cells[goal.y * _size.x + goal.x]->set_wall(EMazeDirection::RIGHT, false);
}




void Maze::make_maze_random_walk()
{
	sf::Vector2u start(0, _size.y / 2);
	sf::Vector2u goal(_size.x - 1, _size.y / 2);

	make_maze_random_walk(start, goal);
	_needs_redraw = true;
}




std::vector<std::shared_ptr<MazeCell>> get_cell_neighbors(std::vector<std::shared_ptr<MazeCell>>& cells, sf::Vector2u pos, sf::Vector2u size)
{
	int x = pos.x;
	int y = pos.y;

	std::vector<std::shared_ptr<MazeCell>> nbs;
	nbs.reserve(4);
	
	if (y > 0)
		nbs.push_back(cells[(y - 1) * size.x + x]);

	if (y + 1 < size.y)
		nbs.push_back(cells[(y + 1) * size.x + x]);

	if (x > 0)
		nbs.push_back(cells[y * size.x + x - 1]);

	if (x + 1 < size.x)
		nbs.push_back(cells[y * size.x + x + 1]);

	return nbs;
}

void Maze::reset()
{
	int width = _size.x;
	int height = _size.y;

	int win_width = _p_win->getSize().x;
	int win_height = _p_win->getSize().y;

	for (int y = 0; y < _size.y; y++)
	{
		for (int x = 0; x < _size.x; x++)
		{
			_cells[y * _size.x + x] = std::make_shared<MazeCell>(MazeCell(sf::Vector2u(x, y), sf::Vector2f(_block_width, _block_height)));
		}
	}

	_needs_redraw = true;
}


/*
for (int y = 0; y < height; y++)
{
	for (int x = 0; x < width; x++)
	{

	}
}
*/