#pragma once

class BoardState;
class Move;

class BoardManager {
public:
	BoardManager();
	~BoardManager();

	string toString();

	bool isGameEnd();

	BoardState* getState() { return state; };
	bool makeMove(Move move);

private:
	Logger * log;
	BoardState *state;

	void setInitialState();

	char** stateToBoard();
};