#pragma once
#include <iostream>
#include <Windows.h>
#include "Debugger.h"
#include <map>

enum EventType {
	PRESSED,
	RELEASED
};

enum MouseButton {
	LEFT_CLICK,
	RIGHT_CLICK
};

class Core;

class Input {
	friend class Window;
private:
	static Input* instance;

	Debugger* dbg;
	std::map<int, EventType> keyStates;
	std::map<MouseButton, EventType> mouseButtons;

	void SetKeyState(char key, EventType state);
	void SetMouseButtonState(MouseButton button, EventType state);

	char key;

	Core* core;

	HWND hwnd;

	int centerX, centerY;

	bool cursorMove;
	float deltaX, deltaY, lastX, lastY;
	
	bool* quit;
public:
	Input();
	POINT CursorPos;

	HCURSOR cursor;
	

	static Input* GetInstance();
	bool GetKeyDown(char key);
	bool GetKeyUp(char key);
	bool GetKey(char key, EventType state);

	bool GetMouseButton(MouseButton button, EventType state);
	bool GetMouseButtonDown(MouseButton button);
	bool GetMouseButtonUp(MouseButton button);

	float GetAxis(std::string axis);
	
	bool TranslateInput(UINT uMsg, WPARAM wParam, LPARAM lParam);
	void RemoveReleased();

	void HideCursor(bool enabled);

	void SetQuit(bool* quit);

	void Init();
};