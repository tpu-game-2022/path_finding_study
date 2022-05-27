#pragma once

class Point
{
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

	Point GetRight() const { return Point(x_ + 1, y_); }
	Point GetLeft() const { return Point(x_ - 1, y_); }
	Point GetUp() const { return Point(x_, y_ - 1); }
	Point GetDown() const { return Point(x_, y_ + 1); }

	// “ñŸŒ³“I‚È’¼ü‹——£
	static double distance(const Point p1, const Point p2)
	{
		double dx = (double)p2.x() - (double)p1.x();
		double dy = (double)p2.y() - (double)p1.y();
		return sqrt(dx * dx + dy * dy);
	}
};