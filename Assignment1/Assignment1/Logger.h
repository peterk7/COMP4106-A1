#pragma once

class SingletonDestroyer;

class Logger {
public:
	static Logger* getInstance(string path = "");
	void info(string str);
	~Logger();

private:
	friend class SingletonDestroyer;

	string path;
	ofstream logFile;
	static Logger* instance;
	static SingletonDestroyer _destroyer;

	Logger(string path);
};

class SingletonDestroyer
{
public:
	SingletonDestroyer(Logger * = 0);
	~SingletonDestroyer();
	void SetSingleton(Logger *s);

private:
	Logger * _singleton;
};