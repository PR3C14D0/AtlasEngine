#pragma once
#include <iostream>
#include <vector>
#include <Windows.h>

class Editor;

class Debugger {
private:
	static Debugger* instance;
	HWND hwnd;

	std::vector<std::string> dbgMsgs;
public:
	void SetHWND(HWND& hwnd);
	Debugger() = default;
	static Debugger* GetInstance();

	void GetMessages(std::vector<std::string>& msgs);

	void Throw(std::string msg);
	void Log(std::string msg);
};