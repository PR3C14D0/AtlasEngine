#pragma once
#include <iostream>
#include <Windows.h>
#include "Engine/Debugger.h"
#include "Engine/Core.h"
#include "Module/Time.h"
#include <time.h>
#include "Engine/Input.h"

class Window {
private:
	HWND hwnd;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Time* time;

	Debugger* dbg;
public:
	Window() = default;
	Window(HINSTANCE& hInstance, HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int& nShowCmd);
};