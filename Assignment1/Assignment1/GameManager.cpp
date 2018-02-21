#include "stdafx.h"

#include "GameManager.h"

GameManager::GameManager() {
	log = Logger::getInstance();
	log->info("GameManager Const");

	board = new BoardManager();
	view = new View();
	ai = new AI();
};

GameManager::~GameManager() {
	log->info("GameManager DTor");
	delete board;
	delete view;
	delete ai;
}

void GameManager::startGame() {
	log->info("GameManager startGame");

	view->displayBoard(board);

	// Analyze path to goal
	log->info("Start analyzing, this may take time");
	clock_t begin = clock();
	list<Move> moveList = ai->analyze(board);
	clock_t end = clock();

	if (moveList.empty()) {
		log->info("Fail");
		return;
	}
	else {
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
		log->info("Success, it took " + to_string(elapsed_secs) + "s.\nPress enter key to start:");
		getchar();
	}

	while (!board->isGameEnd()) {

		// Make next move
		if (!moveList.empty()) {
			Move move = moveList.front();
			moveList.pop_front();
			board->makeMove(move);
		}

		view->displayBoard(board);

		Sleep(500);
	}

	log->info("Game ended");
}