#include "board.h"
bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}

double Point::distance(const Point p1, const Point p2)
{
	double dx = (double)p2.x() - (double)p1.x();
	double dy = (double)p2.y() - (double)p1.y();

	return sqrt(dx * dx + dy * dy);
}

bool Board::isValidated(const Point& p)
{
	if (getMass(p).getStatus() == Mass::WALL)
	{
		return false;
	}
	return true;
}

bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	mass_start.setPos(start.x(), start.y());
	openList.clear();
	openList.push_back(&mass_start);

	while (!openList.empty())
	{
		std::sort(openList.begin(), openList.end(), asc);
		auto it = openList.begin();
		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL)
		{
			Mass* p = current;
			while (p)
			{
				if (p->getStatus() != Mass::START && p->getStatus() != Mass::GOAL)
				{
					p->setStatus(Mass::WAYPOINT);
				}
				p = p->getParent();
			}
			return true;
		}
		else
		{	
			openList.erase(it);
			current->setListed(Mass::CLOSE);
			
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto& c : next)
			{
				if (c.x() < 0 || BOARD_SIZE <= c.x())continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y())continue;
				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) && !m.isListed(Mass::CLOSE) && m.getStatus() != Mass::WALL)
				{
					m.setPos(c.x(), c.y());
					openList.push_back(&m);
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}
	return false;
}

void Board::show() const 
{
	std::cout << std::endl;

	for (int y = 0; y < BOARD_SIZE; y++) {
		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "+-";
		}
		std::cout << "+" << std::endl;

		std::cout << " ";
		for (int x = 0; x < BOARD_SIZE; x++) {
			std::cout << "|";
			switch (mass_[y][x].getStatus()) {
			case Mass::BLANK:
				std::cout << " ";
				break;
			case Mass::START:
				std::cout << "S";
				break;
			case Mass::GOAL:
				std::cout << "G";
				break;
			case Mass::WAYPOINT:
				std::cout << "o";
				break;
			case Mass::WALL:
				std::cout << "#";
				break;
			case Mass::WATER:
				std::cout << "~";
				break;
			case Mass::ROAD:
				std::cout << "$";
				break;
			}
		}
		std::cout << "|" << std::endl;
	}

	std::cout << " ";
	for (int x = 0; x < BOARD_SIZE; x++) {
		std::cout << "+-";
	}
	std::cout << "+" << std::endl;

}


