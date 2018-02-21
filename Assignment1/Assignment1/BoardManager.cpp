#include "stdafx.h"

BoardManager::BoardManager() {
	log = Logger::getInstance();
	log->info("BoardManager Const");

	state = new BoardState();

	setInitialState();
}

BoardManager::~BoardManager() {
	log->info("BoardManager Dtor");
	delete state;
}

void BoardManager::setInitialState() {
	// Middle Solution pegs
	
	state->addPeg(new Point(2, 6));

	state->addPeg(new Point(3, 1));
	state->addPeg(new Point(3, 2));
	state->addPeg(new Point(3, 5));

	state->addPeg(new Point(4, 1));
	state->addPeg(new Point(4, 2));
	state->addPeg(new Point(4, 3));
	state->addPeg(new Point(4, 5));

	state->addPeg(new Point(5, 1));
	state->addPeg(new Point(5, 4));
	state->addPeg(new Point(5, 5));

	state->addPeg(new Point(6, 5));
	state->addPeg(new Point(6, 6));

	state->addPeg(new Point(7, 5));
	

	// Full board state
	/*
	state->addPeg(new Point(1, 3));
	state->addPeg(new Point(1, 4));
	state->addPeg(new Point(1, 5));

	state->addPeg(new Point(2, 2));
	state->addPeg(new Point(2, 3));
	state->addPeg(new Point(2, 4));
	state->addPeg(new Point(2, 5));
	state->addPeg(new Point(2, 6));

	state->addPeg(new Point(3, 1));
	state->addPeg(new Point(3, 2));
	state->addPeg(new Point(3, 3));

	state->addPeg(new Point(3, 5));
	state->addPeg(new Point(3, 6));
	state->addPeg(new Point(3, 7));

	state->addPeg(new Point(4, 1));
	state->addPeg(new Point(4, 2));
	state->addPeg(new Point(4, 3));
	state->addPeg(new Point(4, 4));
	state->addPeg(new Point(4, 5));
	state->addPeg(new Point(4, 6));
	state->addPeg(new Point(4, 7));

	state->addPeg(new Point(5, 1));
	state->addPeg(new Point(5, 2));
	state->addPeg(new Point(5, 3));
	state->addPeg(new Point(5, 4));
	state->addPeg(new Point(5, 5));
	state->addPeg(new Point(5, 6));
	state->addPeg(new Point(5, 7));

	state->addPeg(new Point(6, 2));
	state->addPeg(new Point(6, 3));
	state->addPeg(new Point(6, 4));
	state->addPeg(new Point(6, 5));
	state->addPeg(new Point(6, 6));

	state->addPeg(new Point(7, 3));
	state->addPeg(new Point(7, 4));
	state->addPeg(new Point(7, 5));
	*/
}

string BoardManager::toString() {
	string boardString = "";
	char** board = stateToBoard();

	for (int i = 0; i < state->getRows(); i++) {
		for (int j = 0; j < state->getColumns(); j++) {
			boardString += string(1, board[i][j]) + " ";
		}
		boardString += "\n";
	}

	return boardString;
}

char** BoardManager::stateToBoard() {
	char** board = new char*[state->getRows()];
	for (int i = 0; i < state->getRows(); i++) {
		board[i] = new char[state->getColumns()];
		for (int j = 0; j < state->getColumns(); j++) {
			board[i][j] = BOARD_EMPTY;
		}
	}

	list<Point> *borders = state->getBorders();

	for (std::list<Point>::iterator it = borders->begin();it != borders->end();++it)
	{
		board[it->x][it->y] = BOARD_BORDER;
	}

	list<Point>* pegs = state->getPegList();

	for (std::list<Point>::iterator it = pegs->begin();it != pegs->end();++it)
	{
		board[it->x][it->y] = BOARD_PEG;
	}

	return board;
}

bool BoardManager::isGameEnd() {
	return state->isEndGoal();
}

bool BoardManager::makeMove(Move move) {
	return state->applyMove(move);
}