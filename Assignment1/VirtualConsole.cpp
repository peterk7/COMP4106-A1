#include "stdafx.h"

VirtualConsole::VirtualConsole(const std::string &name, unsigned long color) {
	this->good = 0;
	SECURITY_ATTRIBUTES sa;
	sa.nLength = sizeof(SECURITY_ATTRIBUTES);
	sa.bInheritHandle = 1;
	sa.lpSecurityDescriptor = 0;
	if (!CreatePipe(&this->far_end, &this->near_end, &sa, 0)) {
		assert(this->near_end == INVALID_HANDLE_VALUE);
		return;
	}
	SetHandleInformation(this->near_end, HANDLE_FLAG_INHERIT, 0);
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	ZeroMemory(&pi, sizeof(pi));
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(STARTUPINFO);
	si.hStdInput = this->far_end;
	si.dwFlags |= STARTF_USESTDHANDLES;
	TCHAR program[] = TEXT("console.exe");
	TCHAR arguments[100];
#ifndef UNICODE
	sprintf(arguments, 100, "%d", color);
#else
	swprintf(arguments, 100, L"0 %d", color);
#endif
	if (!CreateProcess(program, arguments, 0, 0, 1, CREATE_NEW_CONSOLE | CREATE_UNICODE_ENVIRONMENT, 0, 0, &si, &pi))
		return;
	this->process = pi.hProcess;
	CloseHandle(pi.hThread);
	this->good = 1;

	this->put(name);
	this->put("\n", 1);
}

VirtualConsole::~VirtualConsole() {
	if (this->near_end != INVALID_HANDLE_VALUE) {
		if (this->process != INVALID_HANDLE_VALUE) {
			TerminateProcess(this->process, 0);
			CloseHandle(this->process);
		}
		CloseHandle(this->near_end);
		CloseHandle(this->far_end);
	}
}

void VirtualConsole::put(const char *str, size_t size) {
	if (!this->good)
		return;
	if (!size)
		size = strlen(str);
	DWORD l;
	WriteFile(this->near_end, str, size, &l, 0);
}