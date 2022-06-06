/*
�Q�lURL
https://qiita.com/2dgames_jp/items/f29e915357c1decbc4b7
https://ja.wikipedia.org/wiki/A*
https://yttm-work.jp/algorithm/algorithm_0015.html
*/


#include "board.h"
#include <algorithm>



//p���s����ꏊ�����ׂ�

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

		int diff_x = abs(p.x() - goal.x());//x�̍�
		int diff_y = abs(p.y() - goal.y());//y�̍�
		if (diff_x > diff_y) {
			if (p.x() < goal.x()) {
				p.setX(next.x() + 1); if (isValidated(next)) { p = next; continue; }
			}//�S�[�����E��������ΉE�Ɉړ�
			if (goal.x() < p.x()) { next.setX(p.x() - 1); if (isValidated(next)) { p = next; continue; } }//�����ɂ���΍��Ɉړ�
		}//��x����@�@��y����
		if (p.y() < goal.y()) { next.setY(p.y() + 1); if (isValidated(next)) { p = next; continue; } }//�㑤�Ȃ��
		if (goal.y() < p.y()) { next.setY(p.y() - 1); if (isValidated(next)) { p = next; continue; } }//�����Ȃ牺

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

	for (int i = 0; i < 100; i++) {//���x���J��Ԃ��ă_���Ȃ炠����߂�
		int step_max = i >> 2;//���x�����߂Ȃ���������𑝂₷

		int r = rand() / ((RAND_MAX + 1) / 4);
		next.set(
			p.x() + ((r == 0) ? 1 : ((r == 1) ? -1 : 0)),
			p.y() + ((r == 2) ? 1 : ((r == 3) ? -1 : 0)));

		if (isValidated(next)) {
			if (step_max < ++step)return true;
			p = next;
			if (p != start)getMass(p).setStatus(Mass::WAYPOINT);
		}
		else {//�ǂɓ�����΍ŏ������蒼��
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
	//start��goal�̈ʒu���}�X�ŋL��

	mass_start.setStatus(Mass::START);
	mass_goal.setStatus(Mass::GOAL);
	//�}�X�̏�Ԃ��w��

	//�I�[�v�����X�g�ɊJ�n�m�[�h��ǉ�

	open_list_.clear();
	open_list_.push_back(&mass_start);
	//Vector(Mass*)�ō���Ă�H�H

	while (!open_list_.empty())
	{
		std :: sort(open_list_.begin(), open_list_.end(), asc);
		auto it = open_list_.begin();

		Mass* current = *it;
		if (current->getStatus() == Mass::GOAL) {
			//�ړI�n�ł���Όo�H�̊���
			Mass* p = current;//�o�H�̃X�e�[�^�X��Mass::WAYPOINT�ɂ���
			while (p) {
				if (p->getStatus() == Mass::BLANK)
					p->setStatus(Mass::WAYPOINT); p = p->getParent();
			}
			return true;
		}
		else {
			//���݂̃m�[�h���N���[�Y���X�g�Ɉڂ�
			open_list_.erase(it);
			current->setListed(Mass::CLOSE);
			//���݂̃m�[�h�̃���s�M����e�m�[�h�𒲂ׂ�
			const Point& pos = current->getPos();
			Point next[4] = { pos.getRight(),pos.getLeft(),pos.getUp(),pos.getDown() };
			for (auto& c : next) {
				if (c.x() < 0 || BOARD_SIZE <= c.x()) continue;
				if (c.y() < 0 || BOARD_SIZE <= c.y()) continue;
				Mass& m = getMass(c);

				//�}�X�ɓ�����邩��
				if (!m.isListed(Mass::OPEN) &&//open���X�g�Ɋ܂܂�Ă��Ȃ�
					!m.isListed(Mass::CLOSE) &&//�N���[�Y�h���X�g�Ɋ܂܂�Ȃ�
					m.getStatus() != Mass::WALL) {//��Q���łȂ�
					//�I�[�v�����X�g�Ɉڂ��ăR�X�g���v�Z����
					open_list_.push_back(&m);
					m.setParent(current, start ,goal);//�R�X�g������Ă���
					m.setListed(Mass::OPEN);	
				}
			}
		}
	}
	return false;
}

