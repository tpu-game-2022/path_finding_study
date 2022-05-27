#include "board.h"

//ソート用比較関数
bool asc(const Mass* s1, const Mass* s2) {
	return s1->getCost() < s2->getCost();
}

bool Board::find(const Point& start, const Point& goal)
{
	Mass& mass_start = getMass(start);
	Mass& mass_goal = getMass(goal);

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);

	//オープンリストに開始ノードを追加する
	open_list_.clear();
	open_list_.push_back(&mass_start);

	while (!open_list_.empty()){//オープンリストは空ではない
		//現在のノード=オープンリストの最も安価なリスト
		std::sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();
		Mass *current = *it;

		if (current->getStatus() == Mass::GOAL) {//目的地なら経路の完成

			Mass* p = current;//経路のステータスをMass::WAYPOINTにする
			while (p)
			{
				if (p->getStatus() != Mass::START&& p->getStatus() != Mass::GOAL) {
					p->setStatus(Mass::WAYPOINT);
				}
				p = p->getParent();
			}
			return true;
		}
		else {
			//現在のノードをクローズリストに移す
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//現在のノードの隣接する各ノードを調べる。
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto& c : next) {//隣接ノード マップ外ならスキップ
				if (c.x() < 0 || BOARD_SIZE <= c.x()) {
					continue;
				}
				if (c.y() < 0 || BOARD_SIZE <= c.y()) {
					continue;
				}
				Mass& m = getMass(c);
				if (!m.isListed(Mass::OPEN) &&//オープンリストに含まれていない
					!m.isListed(Mass::CLOSE) &&//クローズリストに含まれていない
					m.getStatus() != Mass::WALL) {//障害物でない
					open_list_.push_back(&m);//オープンリストに移してコストを計算する
					m.setParent(current, goal);
					m.setListed(Mass::OPEN);
				}
			}
		}
	}
	return false;//到達しなかった。
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
