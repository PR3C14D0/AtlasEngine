#pragma once
#include <Windows.h>

class Input {
private:
	static Input* instance;
public:
	Input() = default;
	static Input* GetInstance();
	void Receive(UINT uMsg);
};