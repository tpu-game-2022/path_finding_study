/*
参考URL
https://qiita.com/2dgames_jp/items/f29e915357c1decbc4b7
https://ja.wikipedia.org/wiki/A*
https://yttm-work.jp/algorithm/algorithm_0015.html
*/


#include "board.h"
#include <algorithm>



//pが行ける場所か調べる

bool Board::find(const Point& start, const Point& goal)
{
	getMass(start).setStatus(Mass::START);
	getMass(goal).setStatus(Mass::GOAL);
		
	Point p = start;
	while (p != goal) {
		Mass& m = getMass(p);
		Mass::status status = m.getStatus();
		if (p != start) m.setStatus(Mass::WAYPOINT);
		Point next = p;

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

		if (random_walk(p, next, start))goto next_loop;

		return false;

	next_loop:
		p = next;

	}
	return true;
}

bool Board :: random_walk(const Point& p0, Point& next, const Point& start) {
	int step = 0;
	Point p = p0;

	for (int i = 0; i < 100; i++) {//何度か繰り返してダメならあきらめる
		int step_max = i >> 2;//何度もだめなら歩く歩数を増やす

		int r = rand() / ((RAND_MAX + 1) / 4);
		next.set(
			p.x() + ((r == 0) ? 1 : ((r == 1) ? -1 : 0)),
			p.y() + ((r == 2) ? 1 : ((r == 3) ? -1 : 0)));

		if (isValidated(next)) {
			if (step_max < ++step)return true;
			p = next;
			if (p != start)getMass(p).setStatus(Mass::WAYPOINT);
		}
		else {//壁に当たれば最初からやり直し
			step = 0;
			p = p0;
		}
	}
	return false;
}

void Board::Astar() {

}

bool Board::listin(const Point& start, const Point& goal) {

	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);
	//startとgoalの位置をマスで記憶

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);
	//マスの状態を指示

	//オープンリストに開始ノードを追加

	open_list_.clear();
	open_list_.push_back(&mass_start);
	//Vector(Mass*)で作ってる？？

	while (!open_list_.empty())
	{
		std :: sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();

		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL) {
			//目的地であれば経路の完成
			Mass* p = current;//経路のステータスをMass::WAYPOINTにする
			while (p) {
				if (p->getStatus() == Mass::BLANK)
					p->setStatus(Mass::WAYPOINT); p = p->getParent();
			}
			return true;
		}
		else {
			//現在のノードをクローズリストに移す
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//現在のノードのリンs熱する各ノードを調べる
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto& c : next) {
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y()) continue;
				Mass& m = getMass(c);

				//マスに入れられるか↓
				if (!m.isListed(Mass::OPEN) &&//openリストに含まれていない
					!m.isListed(Mass::CLOSE) &&//クローズドリストに含まれない
					m.getStatus() != Mass::WALL) {//障害物でない
					//オープンリストに移してコストを計算する
					open_list_.push_back(&m);
					m.setParent(current, start ,goal);//コストも取っている
					m.setListed(Mass::OPEN);	
				}
			}
		}
	}
	return false;
}

