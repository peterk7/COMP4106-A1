#pragma once

class BoardManager;
class View;
class AI;

class GameManager {
public:
	GameManager();
	~GameManager();

	void startGame();

private:
	Logger * log;
	BoardManager * board;
	View * view;
	AI *ai;
};