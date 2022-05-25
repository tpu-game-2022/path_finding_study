#include "board.h"

//pが行ける場所か調べる

bool Board::find(const Point& start, const Point& goal)
{
	getMass(start).setStatus(Mass::START);
	getMass(goal).setStatus(Mass::GOAL);

	Point p = start;
	while (p != goal) {
		Mass &m = getMass(p);
		Mass::status status = m.getStatus();
		if (p != start) m.setStatus(Mass::WAYPOINT);
		Point next;

		int diff_x = abs(p.x() - goal.x());//xの差
		int diff_y = abs(p.y() - goal.y());//yの差
		if (diff_x > diff_y) {
			if (p.x() < goal.x()) {
				p.setX(next.x() + 1); if (isValidated(next)) { p = next; continue; }
			}//ゴールが右側いあれば右に移動
			if (goal.x() < p.x()) { next.setX(p.x() - 1); if (isValidated(next)) { p = next; continue; } }//左側にあれば左に移動
		}//↑xが先　　↓yが先
		if (p.y() < goal.y()) { next.setY(p.y() + 1); if (isValidated(next)) { p = next; continue; } }//上側なら上
		if (goal.y() < p.y()) { next.setY(p.y() - 1); if (isValidated(next)) { p = next; continue; } }//下側なら下
	
		return false;

	}
	return true;
}
