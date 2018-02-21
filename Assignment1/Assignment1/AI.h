#pragma once

struct CustomHash {
public:
	size_t operator()(const BoardState *bs) const {
		static const size_t shift = (size_t)log2(1 + sizeof(bs));
		return (size_t)(bs) >> shift;
	}
};

struct CustomEqual {
public:
	bool operator()(const BoardState *lhs, const BoardState *rhs) const {

		return lhs->isEqual(rhs);
	}
};

class AI {
public:
	AI();
	~AI();

	list<Move> analyze(BoardManager *board);

	list<Move> BFSSearch(BoardState* fringe);
	list<Move> DFSSearch(BoardState* fringe);
	list<Move> AStarSearch(BoardState* fringe);

	list<BoardState*> ExpandState(BoardState * node);
	list<Move> extractMoves(BoardState *goalState);

private:
	Logger * log;
	unordered_set<BoardState*, CustomHash, CustomEqual> *visited;
	int maxPSSize;
	int nodeCount;
	int skippedNodeCount;

	BoardState *startState;

	void emptyVisited();
	bool isInVisited(BoardState *bs);
};