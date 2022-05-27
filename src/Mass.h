#pragma once
#include "Point.h"

class Mass
{
public:
	enum status
	{
		BLANK,
		GOAL,
		START,
		WAYPOINT,
		WALL, // 通れない
		WATER,// 進むのが1/3に遅くなる
		ROAD,//進むのが3倍速い
	};
	enum listed
	{
		NONE,
		OPEN,
		CLOSE,
	};
private:
	status s_ = BLANK;
	listed listed_ = NONE;   // リストに登録されているかを高速に判断するために自ら所持
	Point pos_;   // 親を持てるようにする
	Mass* pParent_ = nullptr;
	int steps_ = 0;
	double estimate_ = 0.0;

	static int getWalkCost(Mass& m)
	{
		status s = m.getStatus();
		int cost = 3;   // BLANKのコスト：3

		if (s == ROAD)
		{
			cost /= 3;
		}
		else if (s == WATER)
		{
			cost *= 3;
		}

		return cost;
	}

	void calcCost(const Point target)
	{
		// steps_ = (pParent_ ? pParent_->steps_ : 0) + 1;
		steps_ = (pParent_ ? pParent_->steps_ : 0) + getWalkCost(*this);
		estimate_ = Point::distance(pos_, target);
	}
public:
	void setStatus(status s) { s_ = s; }
	status getStatus() const { return s_; }

	void setPos(int x, int y) { pos_.set(x, y); }
	const Point& getPos() const { return pos_; }
	int x() { return pos_.x(); }
	int y() { return pos_.y(); }

	// 親を持てるようにする
	void setParent(Mass* pParent, const Point& goal) { pParent_ = pParent; calcCost(goal); }
	Mass* getParent() { return pParent_; }

	void setListed(listed t) { listed_ = t; }
	bool isListed(listed t) const { return listed_ == t; }

	double getCost() const { return (double)steps_ + estimate_; }
};

// ソート用比較関数、board.cppで定義
bool asc(const Mass* o1, const Mass* o2);