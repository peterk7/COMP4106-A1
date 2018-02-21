#pragma once

#include "stdafx.h"

class VirtualConsole {
	HANDLE near_end, //Our end of the pipe, where we'll write.
		far_end, //The other end.
		process; //The client.
public:
	bool good;
	VirtualConsole(const std::string &name, unsigned long color);
	~VirtualConsole();
	void put(const char *str, size_t size = 0);
	void put(const std::string &str) {
		this->put(str.c_str(), str.size());
	};
};