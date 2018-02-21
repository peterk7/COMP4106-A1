#include "stdafx.h"

list<Point>* BoardState::borders = NULL;

BoardState::BoardState() {
	log = Logger::getInstance();
	log->info("BoardState Ctor");

	parent = NULL;
	lastMove = NULL;
	rows = 9;
	columns = 9;
	isolatedPegCout = 0;
	costSoFar = 0;
	movablePegs = 0;
	setBorders();

	pegs = new list<Point>();
}

BoardState::BoardState(const BoardState *other) {
	log = other->log;
	//log->info("BoardState CCtor");
	parent = other->parent;
	lastMove = other->lastMove;
	rows = other->rows;
	columns = other->columns;
	isolatedPegCout = other->isolatedPegCout;
	costSoFar = other->costSoFar;
	movablePegs = other->movablePegs;

	pegs = new list<Point>();

	for (std::list<Point>::iterator it = other->pegs->begin();it != other->pegs->end();++it) {
		pegs->push_back(Point(it->x, it->y));
	}
}

BoardState::~BoardState() {
	log->info("BoardState Dtor");
	delete lastMove;
	delete pegs;
}

void BoardState::addPeg(Point *p) {
	pegs->push_back(*p);
	calcIsoletedPegsCount();
}

void BoardState::setBorders() {
	if (borders != NULL) {
		return;
	}

	borders = new list<Point>();

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			if (0 == i || (rows - 1) == i) {
				borders->push_back(Point(i, j));
			}
			else  if (0 == j || (columns - 1) == j) {
				borders->push_back(Point(i, j));
			}
		}
	}

	borders->push_back(Point(1, 1));
	borders->push_back(Point(1, 2));
	borders->push_back(Point(2, 1));

	borders->push_back(Point(1, 6));
	borders->push_back(Point(1, 7));
	borders->push_back(Point(2, 7));

	borders->push_back(Point(6, 1));
	borders->push_back(Point(7, 1));
	borders->push_back(Point(7, 2));

	borders->push_back(Point(6, 7));
	borders->push_back(Point(7, 6));
	borders->push_back(Point(7, 7));

}

bool BoardState::isEqual(const BoardState *other) const {

	if (pegs->size() != other->pegs->size()) {
		return false;
	}

	for (std::list<Point>::iterator it = pegs->begin();it != pegs->end();++it)
	{
		auto findIter = find(other->pegs->begin(), other->pegs->end(), *it);
		if (findIter == other->pegs->end()) {
			return false;
		}
	}

	return true;
}

bool BoardState::isEndGoal() {
	return pegs->size() <= 1;
}

list<Move> BoardState::getPossibleMoves() {
	list<Move> moves;

	for (std::list<Point>::iterator it = pegs->begin();it != pegs->end();++it)
	{
		Point to;
		Point remove;

		// Right
		to = Point((it->x), it->y + 2);
		remove = Point((it->x), it->y + 1);
		if (getInPosition(remove) == BOARD_PEG && getInPosition(to) == BOARD_EMPTY) {
			Move m = Move(*it, to, remove);
			moves.push_back(m);
		}

		// Bottom
		to = Point((it->x) + 2, it->y);
		remove = Point((it->x) + 1, it->y);
		if (getInPosition(remove) == BOARD_PEG && getInPosition(to) == BOARD_EMPTY) {
			Move m = Move(*it, to, remove);
			moves.push_back(m);
		}

		// Left
		to = Point((it->x), it->y - 2);
		remove = Point((it->x), it->y - 1);
		if (getInPosition(remove) == BOARD_PEG && getInPosition(to) == BOARD_EMPTY) {
			Move m = Move(*it, to, remove);
			moves.push_back(m);
		}

		// Top
		to = Point((it->x) - 2, it->y);
		remove = Point((it->x) - 1, it->y);
		if (getInPosition(remove) == BOARD_PEG && getInPosition(to) == BOARD_EMPTY) {
			Move m = Move(*it, to, remove);
			moves.push_back(m);
		}
	}

	return moves;
}

char BoardState::getInPosition(Point p) {
	if (p.x < 0 || p.x > rows || p.y < 0 || p.y > columns) {
		return BOARD_BORDER;
	}

	list<Point>::iterator findIter = find(borders->begin(), borders->end(), p);
	if (findIter != borders->end()) {
		return BOARD_BORDER;
	}

	findIter = find(pegs->begin(), pegs->end(), p);
	if (findIter != pegs->end()) {
		return BOARD_PEG;
	}

	return BOARD_EMPTY;
}

bool BoardState::applyMove(Move m) {
	// Check if move possible

	// Remove m.remove
	pegs->remove(m.remove);
	// Remove m.from
	pegs->remove(m.from);
	// Add m.to
	pegs->push_back(m.to);
	lastMove = new Move(m);

	calcIsoletedPegsCount();
	calcMovablePegs();
	costSoFar++;

	return true;
}

void BoardState::calcIsoletedPegsCount() {
	int count = 0;

	for (std::list<Point>::iterator it = pegs->begin();it != pegs->end();++it)
	{
		Point top = Point((it->x) - 1, it->y);
		Point bottom = Point((it->x) + 1, it->y);
		Point left = Point((it->x), (it->y) - 1);
		Point right = Point((it->x), (it->y) + 1);

		if (getInPosition(top) == BOARD_PEG || getInPosition(bottom) == BOARD_PEG || getInPosition(left) == BOARD_PEG || getInPosition(right) == BOARD_PEG) {
			continue;
		}
		count++;
	}

	isolatedPegCout = count;
}

void BoardState::calcMovablePegs() {
	list<Move> pmoves = getPossibleMoves();

	struct CustomHash {
	public:
		size_t operator()(const Point p) const {
			hash<int> hashVal;
			return hashVal(p.x) + hashVal(p.y);
		}
	};

	unordered_set<Point, CustomHash> pegsLst;

	for (auto it = pmoves.begin(); it != pmoves.end(); ++it) {
		pegsLst.insert(it->from);
	}

	movablePegs = pegsLst.size();
}