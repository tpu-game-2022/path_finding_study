#pragma once
#include <iostream>
#include <vector>



inline int pow(int num)
{
	return num * num;
}//二乗を返す　何故かusing.systemが使えなかったため作成

class Point {
	int x_ = -1,
		y_ = -1;
public:
	Point() {}
	Point(int x, int y) :x_(x), y_(y) {}
	int x() const { return x_; }
	int y() const { return y_; }
	void setX(int x) { x_ = x; }
	void setY(int y) { y_ = y; }
	void set(int x, int y) { x_ = x; y_ = y; }

	bool operator == (const Point& p) const {
		return p.x() == x_ && p.y() == y_;
	}
	bool operator != (const Point& p) const {
		return !(p == *this);
	}

	Point getRight()const { return Point(x_ + 1, y_); }
	Point getLeft()const { return Point(x_ - 1, y_); }
	Point getUp()const { return Point(x_, y_ - 1); }
	Point getDown()const { return Point(x_, y_ + 1); }
};


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
	enum listed {
		NONE, 
		OPEN, 
		CLOSE,
	};
private:
	status s_ = BLANK;
	listed listed_ = NONE;
	Point pos_;
	Mass* pParent_ = nullptr;
	int steps_ = 0;
	double estimate_ = 0.0;

	void calcCost(Mass* pParent , const Point& start, const Point target) {
		steps_ = (pParent_ ? pParent_->steps_ : 0) + 1;
		const Point& pos = pParent->getPos();
		estimate_ = WhatCost(pos, start, target);
	}
public:
	Mass() {}
	~Mass() {}
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }

	void setPos(int x, int y) { pos_.set(x, y); }
	const Point& getPos()const { return pos_; }
	int x() { return pos_.x(); }
	int y() { return pos_.y(); }

	void setParent(Mass* pParent, const Point& start, const Point& goal) { pParent_ = pParent; calcCost(pParent,start,goal); }
	Mass* getParent() { return pParent_; }

	void setListed(listed t) { listed_ = t; }
	bool isListed(listed t)const { return listed_ == t; }

	double getCost()const { return (double)steps_ + estimate_; }
	//↑estimate が　負荷コスト

	float WhatCost(const Point& p, const Point& start, const Point& goal){
		float C = abs(start.y() - p.y()) + abs(start.x() - p.x());//実コスト　　現在マスからの移動コスト
		float H = sqrt(pow(abs(goal.y() - p.y())) + pow(abs(goal.x() - p.x())));//推定コスト　ゴールマスへの予測コスト
		float M;// マスコスト　マス自体のコスト
		if (getStatus() == Mass::ROAD)M = 1/3;
		else if (getStatus() == Mass::WATER)M = 3;

		return (C + H) * M;//合計スコア
	}

};



inline bool asc(const Mass* o1, const Mass* o2) {
	return o1 -> getCost() < o2 -> getCost();
}

class Board {
private:
	enum {
		BOARD_SIZE = 10,
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];

	//getMass（Getを生成　Mass型関数　
	//ポイントの引き数をもらって配列（座標）を返す）
	Mass& getMass(const Point p) { return mass_[p.y()][p.x()]; }

	std::vector<Mass*> open_list_;
	
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
	bool random_walk(const Point& p0, Point& next, const Point& start);
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

	//A*アルゴリズムの経路探索
	void Astar();
	bool listin(const Point& start, const Point& goal);

};
