#include "board.h"
#include <algorithm>

bool asc(const Mass* o1, const Mass* o2)
{
	return o1->getCost() < o2->getCost();
}
bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	open_List_.clear();
	open_List_.push_back(&mass_start);

	while (!open_List_.empty())
	{
		//asc‚Åƒ\[ƒg
		std::sort(open_List_.begin(), open_List_.end(), asc);
		
		auto it = open_List_.begin();
		
		Mass* current = *it;
		
		if (current->getStatus() == Mass::GOAL)
		{
			Mass* p = current;

			while (p)
			{
				if (p->getStatus() == Mass::BLANK) p->setStatus(Mass::WAYPOINT); p = p->getParent();

			}
			return true;
		}
		else
		{
			open_List_.erase(it);
			current->setListed(Mass::CLOSE);
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };

			Point p;
			for (auto& c : next)
			{

				if (c.x() < 0 || BOARD_SIZE <= c.x())continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y())continue;

				Mass& mass = getMass(c);

				if (!mass.isListed(Mass::OPEN) && !mass.isListed(Mass::CLOSE) && mass.getStatus() != Mass::WALL)
				{
					open_List_.push_back(&mass);
					//WATER
					mass.setParent(current, goal);
					mass.setListed(Mass::OPEN);
				}
			}
		}
	}
	return false;
};
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
