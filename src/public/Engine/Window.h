#pragma once
#include <Windows.h>
#include "Engine/Error.h"
#include "Engine/Core.h"

class Window {
private:
	HWND hwnd;
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Error* error;
public:
	Window() = default;
	Window(HINSTANCE& hInstance, HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int& nShowCmd);
};