#include "stdafx.h"

#pragma warning(disable:4996)

Logger* Logger::instance = 0;
SingletonDestroyer Logger::_destroyer;

Logger::Logger(string newPath) {
	path = newPath;
	if (newPath == "") {
		return;
	}

	logFile.open(path, ios::app | ios::out);

	info("Logger Ctor");
};

Logger::~Logger() {
	info("Logger Dtor");
	logFile << "~~~~~~~~~~" << endl << endl;
	logFile.close();
}

SingletonDestroyer::SingletonDestroyer(Logger *s)
{
	_singleton = s;
}

SingletonDestroyer::~SingletonDestroyer()
{
	delete _singleton;
}

void SingletonDestroyer::SetSingleton(Logger *s)
{
	_singleton = s;
}

Logger* Logger::getInstance(string path) {
	if (instance == 0) {
		instance = new Logger(path);
		_destroyer.SetSingleton(instance);
	}
	return instance;
};

void Logger::info(string str) {

	auto time = chrono::system_clock::now();
	time_t currTime = chrono::system_clock::to_time_t(time);
	string timeStr = ctime(&currTime);
	timeStr = timeStr.substr(0, timeStr.length() - 1);

	string logStr = timeStr + ": " + str;

	cout << logStr << endl;

	if (logFile.is_open()) {
		logFile << logStr << endl;
	}
}