// Assignment1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

int main()
{
	Logger* log = Logger::getInstance("A1.log");

	log->info("Start Main");
	GameManager* g = new GameManager ();

	g->startGame();

	log->info("End Main");
    return 0;
}

