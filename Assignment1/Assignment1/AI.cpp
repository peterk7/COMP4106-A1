#include "stdafx.h"

AI::AI() {
	log = Logger::getInstance();
	log->info("AI Const");

	visited = NULL;
}

AI::~AI() {
	log->info("AI Dtor");

	if (visited != NULL) {
		emptyVisited();
	}
}

list<Move> AI::analyze(BoardManager *board) {
	// reset visited nodes
	if (visited != NULL) {
		emptyVisited();
	}
	visited = new unordered_set<BoardState*, CustomHash, CustomEqual>();

	maxPSSize = 0;

	startState = new BoardState(board->getState());

	//list<Move> m = BFSSearch(startState);
	//list<Move> m = DFSSearch(startState);
	list<Move> m = AStarSearch(startState);
	
	log->info("Analyzed " + to_string(nodeCount) + " states. Skipped " + to_string(skippedNodeCount) + " states, Max PS Size was " + to_string(maxPSSize) + ".");

	return m;
}

list<Move> AI::BFSSearch(BoardState* startState) {
	queue<BoardState*> *fringe = new queue<BoardState*>();
	fringe->push(startState);

	list<Move> moves;

	while (!fringe->empty()) {
		nodeCount++;

		BoardState *node = fringe->front();
		fringe->pop();

		if (node->isEndGoal()) {
			moves = extractMoves(node);
			return moves;
		}

		// Check if seen before
		if (isInVisited(node)) {
			skippedNodeCount++;
			continue;
		}
		else {
			visited->insert(node);
		}


		list<BoardState*> productionSystem = ExpandState(node);

		// fringe.insert(expand(node))
		for (std::list<BoardState*>::iterator it = productionSystem.begin();it != productionSystem.end();++it) {
			fringe->push(*it);
		}
	}

	// TODO: return false / failure
	return moves;
}

list<Move> AI::DFSSearch(BoardState *startState) {
	stack<BoardState*> *fringe = new stack<BoardState*>();
	fringe->push(startState);

	list<Move> moves;

	while (!fringe->empty()) {
		nodeCount++;

		BoardState *node = fringe->top();
		fringe->pop();

		if (node->isEndGoal()) {
			moves = extractMoves(node);
			return moves;
		}

		// Check if seen before
		if (isInVisited(node)) {
			skippedNodeCount++;
			continue;
		}
		else {
			visited->insert(node);
		}

		list<BoardState*> productionSystem = ExpandState(node);

		// fringe.insert(expand(node))
		for (std::list<BoardState*>::iterator it = productionSystem.begin();it != productionSystem.end();++it) {
			fringe->push(*it);
		}
	}

	// TODO: return false / failure
	return moves;
}

list<Move> AI::AStarSearch(BoardState* startState) {

	// Number of possible moves / pegs to move ?

	// Number of isolated pegs - less the better
	class ComparisonClass {
	public:
		bool operator() (BoardState* left, BoardState* right) {
			return (left->getCostSoFar() + left->getMovablePegs()) < (right->getCostSoFar() + right->getMovablePegs());
		}
	};
	priority_queue<BoardState*, vector<BoardState*>, ComparisonClass> *fringe = new priority_queue<BoardState*, vector<BoardState*>, ComparisonClass>();
	fringe->push(startState);

	list<Move> moves;

	while (!fringe->empty()) {
		nodeCount++;

		BoardState *node = fringe->top();
		fringe->pop();

		if (node->isEndGoal()) {
			moves = extractMoves(node);
			return moves;
		}

		// Check if seen before
		if (isInVisited(node)) {
			skippedNodeCount++;
			continue;
		}
		else {
			visited->insert(node);
		}

		list<BoardState*> productionSystem = ExpandState(node);

		for (std::list<BoardState*>::iterator it = productionSystem.begin();it != productionSystem.end();++it) {
			fringe->push(*it);
		}
	}

	// TODO: return false / failure
	return moves;
}

list<BoardState*> AI::ExpandState(BoardState * node) {
	list<BoardState*> ret;

	// Get possible moves
	list<Move> moves = node->getPossibleMoves();

	if (moves.size() > maxPSSize) {
		maxPSSize = moves.size();
	}
	
	// Create new state for each move
	for (std::list<Move>::iterator it = moves.begin();it != moves.end();++it) {
		BoardState *newNode = new BoardState(node);
		// Set parent node
		newNode->parent = node;
		newNode->applyMove(*it);
		ret.push_back(newNode);
	}

	// Return a list of states
	return ret;
}

list<Move> AI::extractMoves(BoardState *goalState) {
	list<Move> moves;
	while (goalState->parent != NULL) {
		moves.push_front(*(goalState->lastMove));
		goalState = goalState->parent;
	}

	return moves;
}

void AI::emptyVisited() {
	for (auto itr = visited->begin(); itr != visited->end(); itr++) {
		delete *itr;
	}
	delete visited;
	visited = NULL;
}

bool AI::isInVisited(BoardState *bs) {
	return visited->find(bs) != visited->end();
}