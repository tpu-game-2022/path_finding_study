#pragma once
#include <iostream>
#include <vector>
#include <algorithm>
#include <map>

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

	Point getRight() const { return Point(x_ + 1, y_); }
	Point getLeft() const { return Point(x_ - 1, y_); }
	Point getUp() const { return Point(x_, y_ - 1); }
	Point getDown() const { return Point(x_, y_ + 1); }

	//2次元的な直線距離
	static double distance(const Point p1, const Point p2)
	{
		double dx = (double)p2.x() - (double)p1.x();
		double dy = (double)p2.y() - (double)p1.y();
		return sqrt(dx * dx + dy * dy);
	}
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
	//リストを管理
	enum listed
	{
		NONE,
		OPEN,
		CLOSE,
	};
private:
	status s_ = Mass::BLANK;
	//リストへ登録されているか高速に判定する為に自ら所持
	listed listed_ = Mass::NONE;
	Point pos_;
	//親を持てるようにする
	Mass* pParent_ = nullptr;
	double steps_ = 0;
	double estimate_ = 0.0;

	//コストに地形の効果追加
	static double getWalkCost(double a,Mass& m)
	{ 
		status s = m.getStatus();
		double cost = 0;

		if (s == WATER)
		{
			//1/3遅くなる(コストを上げる)
			return a * 3.0;
		}
		if (s == ROAD)
		{
			//3倍速くなる(コストを減らす)
			return a / 3.0;
		}

		return 1.0;
	}

	//今までの補数と残りの距離の見積もりでコストを定める
	void calcCost(const Point target)
	{
		auto a = (pParent_ ? pParent_->steps_ : 0);
		steps_ = getWalkCost(a, *this);
		estimate_ = Point::distance(pos_, target);
	}
public:
	Mass(){}
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }

	void setPos(int x, int y) { pos_. set(x, y); }
	const Point& getPos() const { return pos_; }
	int x() { return pos_.x(); }
	int y() { return pos_.y(); }

	//親を持てるようにする
	void setParent(Mass* pParent, const Point& goal) { pParent_ = pParent; calcCost(goal); }
	Mass* getParent() { return pParent_; }

	//リストを格納
	void setListed(listed t) { listed_ = t; }
	//リストを返す
	bool isListed(listed t)const { return listed_ == t; }

	//コストを返却
	double getCost() const { return (double)steps_ + estimate_; }

};

class Board {
private:
	enum {
		BOARD_SIZE = 10,
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];
	Mass& getMass(const Point p) { return mass_[p.y()][p.x()]; }

	std::vector<Mass*> open_list_;
public:
	Board() {
		for (int y = 0; y < BOARD_SIZE; y++) {
			for (int x = 0; x < BOARD_SIZE; x++) {
				mass_[y][x].setPos(x, y);
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
	~Board(){}

	bool isValidated(const Point& p);

	bool find(const Point& start, const Point& goal);

	void show() const;
};