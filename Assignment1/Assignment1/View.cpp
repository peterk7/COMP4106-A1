#include "stdafx.h"

View::View() {
	log = Logger::getInstance();
	log->info("View Const");
}

View::~View() {
	log->info("View Dtor");
}

void View::displayBoard(BoardManager *bm) {
	system("cls");
	cout << bm->toString() << endl;
}