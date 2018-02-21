#pragma once

class Point;
class Move;

class BoardState {
public:
	BoardState * parent;
	Move *lastMove;

	BoardState();
	BoardState(const BoardState *other);
	~BoardState();

	void addPeg(Point *peg);

	int getRows() { return rows; }
	int getColumns() { return columns; }

	int getPegCount() const { return pegs->size(); }
	int getIsoletedPegsCount() { return isolatedPegCout; };
	int getCostSoFar() { return costSoFar; };
	int getMovablePegs() { return movablePegs; };

	list<Point>* getPegList() { return pegs; };
	list<Point>* getBorders() { return borders; };

	bool applyMove(Move m);

	bool isEqual(const BoardState *other) const;

	bool isEndGoal();

	list<Move> getPossibleMoves();

	char getInPosition(Point p);

private:
	Logger * log;
	int rows;
	int columns;
	int isolatedPegCout;
	int costSoFar;
	int movablePegs;
	
	list<Point> *pegs;
	static list<Point> *borders;

	void setBorders();
	void calcIsoletedPegsCount();
	void calcMovablePegs();
};

class Point {
public:
	int x;
	int y;
	Point(int posX = 0, int posY = 0) { x = posX; y = posY; }
	Point(const Point &other) { x = other.x; y = other.y; }
	~Point() {};

	bool operator==(const Point &other) const { return ((x == other.x) && (y == other.y)); };
};

class Move {
public:
	Point from;
	Point to;
	Point remove;

	Move(Point f, Point t, Point r) { from = f; to = t; remove = r; };
	Move(const Move &other) { from = other.from; to = other.to; remove = other.remove; };
	~Move() {};
};