#pragma once

class View {
public:
	View();
	~View();

	void displayBoard(BoardManager *bm);
private:
	Logger * log;
};