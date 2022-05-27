#pragma once
#include <iostream>
#include <vector>
#include "Mass.h"
#include "Point.h"

class Board
{
private:
	enum
	{
		BOARD_SIZE = 10,
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];
	Mass& getMass(const Point p) { return mass_[p.y()][p.x()]; }

	std::vector<Mass*> open_list_;
public:
	Board()
	{
		for (int y = 0; y < BOARD_SIZE; y++)
		{
			for (int x = 0; x < BOARD_SIZE; x++)
			{
				mass_[y][x].setStatus(Mass::BLANK);
				mass_[y][x].setPos(x, y);  // ?
			}
		}
		// 壁
		mass_[4][6].setStatus(Mass::WALL);
		mass_[4][5].setStatus(Mass::WALL);
		mass_[5][5].setStatus(Mass::WALL);
		mass_[6][5].setStatus(Mass::WALL);
		// 水
		for (int y = 4; y <= 7; y++)
		{
			for (int x = 1; x <= 4; x++)
			{
				mass_[y][x].setStatus(Mass::WATER);
			}
		}
		// 道
		mass_[4][1].setStatus(Mass::ROAD);
		mass_[5][1].setStatus(Mass::ROAD);
		mass_[5][2].setStatus(Mass::ROAD);
		mass_[5][3].setStatus(Mass::ROAD);
		mass_[6][1].setStatus(Mass::ROAD);
		mass_[6][2].setStatus(Mass::ROAD);
		mass_[6][3].setStatus(Mass::ROAD);
	}
	~Board() {}

	// pが行ける場所か調べる
	bool isValidated(const Point& p)
	{
		if (getMass(p).getStatus() == Mass::WALL) return false;

		return true;
	}

	bool find(const Point& start, const Point& goal);

	void show() const;
};
