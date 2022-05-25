#pragma once
#include <iostream>


class Mass {
public:
	enum status {
		BLANK,
		GOAL,
		START,
		WAYPOINT,
		WALL, // 通れない
		WATER,// 進むのが1/3に遅くなる
		ROAD,//進むのが3倍速い
	};
private:
	status s_ = BLANK;
public:
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }
};

class Point {
	int x_ = -1,
		y_ = -1;
public:
	Point(int x, int y) :x_(x), y_(y) {}
	int x() const { return x_; }
	int y() const { return y_; }
	void setX(int x) { x_ = x; }
	void setY(int y) { y_ = y; }

	bool operator == (const Point& p) const {
		return p.x() == x_ && p.y() == y_;}
	bool operator != (const Point& p) const {
		return !(p == *this);}
};

class Board {
private:
	enum {
		BOARD_SIZE = 10,
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];

	//getMass（Getを生成　Mass型関数　
	//ポイントの引き数をもらって配列（座標）を返す）
	Mass& getMass(const Point p) { return mass_[p.y()][p.x()]; }
public:
	Board() {
		for (int y = 0; y < BOARD_SIZE; y++) {
			for (int x = 0; x < BOARD_SIZE; x++) {
				mass_[y][x].setStatus(Mass::BLANK);
			}
		}
		// 壁
		mass_[4][6].setStatus(Mass::WALL);
		mass_[4][5].setStatus(Mass::WALL);
		mass_[5][5].setStatus(Mass::WALL);
		mass_[6][5].setStatus(Mass::WALL);
		// 水
		for (int y = 4; y <= 7; y++) {
			for (int x = 1; x <= 4; x++) {
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
	void addWall(const Point& p) { getMass(p).setStatus(Mass::WALL); }

	bool isValidated(const Point& p) {
		if(getMass(p).getStatus() == Mass::WALL) {
			return false;
		}
		return true;
	}

	bool find(const Point& start, const Point& goal);

	void show() const {
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


};
