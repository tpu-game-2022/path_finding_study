#pragma once
#include <iostream>
#include <vector>
#include <algorithm>

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
	Point getLeft()  const { return Point(x_ - 1, y_); }
	Point getUp()    const { return Point(x_, y_ - 1); }
	Point getDown()  const { return Point(x_, y_ + 1); }

	static double distance(const Point p1, const Point p2) {
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

	/*void calcCost(const Point target) {
		steps_ = (pParent_ ? pParent_->steps_ : 0) + 1;
		estimate_ = Point::distance(pos_, target);
	}*/
	static int getWalkCost(Mass& m) {
		status s = m.getStatus();
		if (s == WATER) {
			return 3;
		}
		else if (s == ROAD) {
			return 1 / 3;
		}
		else {
			return 1;
		}
	}


	void calcCost(const Point target) {
		steps_ = (pParent_ ? pParent_->steps_ : 0) + getWalkCost(*this);
		estimate_ = Point::distance(pos_, target);
	}
public:
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }

	void setPos(int x, int y) { pos_.set(x, y); }
	const Point &getPos() const { return pos_; }
	int x() { return pos_.x(); }
	int y() { return pos_.y(); }

	void setParent(Mass* pParent, const Point& goal) { pParent_ = pParent; calcCost(goal); }
	Mass* getParent() { return pParent_; }

	void setListed(listed t) { listed_ = t; }
	bool isListed(listed t) const { return listed_ == t; }

	double getCost() const { return (double)steps_ + estimate_; }
};

//bool asc(const Mass* o1, const Mass* o2) {
//	return o1->getCost() < o2->getCost();
//}

class Board {
private:
	enum {
		BOARD_SIZE = 10,
	};
	Mass mass_[BOARD_SIZE][BOARD_SIZE];
	//Mass& getMass(const Point& p) { getMass(p).setStatus(Mass::WALL); }
	Mass& getMass(const Point& p) { return mass_[p.y()][p.x()]; }

	std::vector<Mass*> open_list_;
public:
	Board() {
		for (int y = 0; y < BOARD_SIZE; y++) {
			for (int x = 0; x < BOARD_SIZE; x++) {
				//mass_[y][x].setStatus(Mass::BLANK);
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
	~Board() {}
	void addWall(const Point& p) { getMass(p).setStatus(Mass::WALL); }

	//p が行ける場所か調べる
	bool isValibated(const Point& p) {
		if (getMass(p).getStatus() == Mass::WALL) {
			return false;
		}
		return true;
	}

	bool find(const Point& start, const Point& goal);
	//{
	//	Mass &mass_start = getMass(start);
	//	Mass& mass_goal = getMass(goal);

	//	getMass(start).setStatus(Mass::START);
	//	getMass(goal).setStatus(Mass::GOAL);

	//	// オープンリストに開始ノードを追加する
	//	open_list_.clear();
	//	open_list_.push_back(&mass_start);

	//	while(!open_list_.empty()) { // オープンリストが空でない
	//		// 現在のノード＝オープンリストの最も安価なリスト
	//		std::sort(open_list_.begin(), open_list_.end(), asc);
	//		auto it = open_list_.begin();
	//		Mass* current = *it;
	//		if (current->getStatus() == Mass::GOAL) {
	//			// 目的地なら経路の完成
	//			Mass* p = current;// 経路のステータスをMass::WATPOINTにする
	//			while (p) {
	//				if (p->getStatus() == Mass::BLANK) p->setStatus(Mass::WAYPOINT); p = p->getParent(); }
	//			return true;
	//		}
	//		else {
	//			// 現在のノードをクローズドリストに移す
	//			open_list_.erase(it);
	//			current->setListed(Mass::CLOSE);
	//			// 現在のノードの隣接する各ノードを調べる
	//			const Point& pos = current->getPos();
	//			Point next[4] = { pos.getRight(), pos.getLeft(), pos.getUp(), pos.getDown() };
	//			for (auto& c : next) {// 隣接ノード
	//				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;// マップ外ならスキップ
	//				if (c.y() < 0 || BOARD_SIZE <= c.y()) continue;
	//				Mass& m = getMass(c);
	//				if (!m.isListed(Mass::OPEN) &&//オープンリストに含まれない
	//					!m.isListed(Mass::CLOSE) && //クローズドリストに含まれない
	//					m.getStatus() != Mass::WALL) {//障害物でない
	//					//オープンリストに移してコストを計算する
	//					open_list_.push_back(&m);
	//					m.setParent(current, goal);
	//					m.setListed(Mass::OPEN);
	//				}
	//			}
	//		}
	//	}
	//	return false;
	//}

	void show() const;
};
