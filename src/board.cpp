#include "board.h"

bool Board::find(const Point& start, const Point& goal)
{
	mass_[start.y()][start.x()].setStatus(Mass::START);
	mass_[goal.y()][goal.x()].setStatus(Mass::GOAL);

	Point p = start;
	while (p != goal) {
		if (p != start) mass_[p.x()][p.y()].setStatus(Mass::WAYPOINT);

		if (p.x() < goal.x()) { p.setX(p.x() + 1); continue; }
		if (goal.x() < p.x()) { p.setX(p.x() - 1); continue; }
		if (p.y() < goal.y()) { p.setY(p.y() + 1); continue; }
		if (goal.y() < p.y()) { p.setY(p.y() - 1); continue; }
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
