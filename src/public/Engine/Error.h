#pragma once
#include <iostream>
#include <Windows.h>

class Error {
private:
	static Error* instance;
	HWND hwnd;
public:
	void SetHWND(HWND& hwnd);
	Error() = default;
	static Error* GetInstance();

	void Throw(std::string msg);
};