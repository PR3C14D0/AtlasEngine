#include "Engine/Input.h"
#include "Engine/Core.h"

Input* Input::instance;

Input::Input() {
	this->dbg = Debugger::GetInstance();
	this->deltaX = 0.f;
	this->deltaY = 0.f;
	this->lastX = 0.f;
	this->lastY = 0.f;
	this->CursorPos = { 0, 0 };

	this->centerX = 0.f;
	this->centerY = 0.f;
	this->hwnd = NULL;
	this->core = nullptr;
}

void Input::SetQuit(bool* quit) {
	this->quit = quit;
}

void Input::Init() {
	this->core = Core::GetInstance();
	this->core->GetHWND(&this->hwnd);
	this->cursor = GetCursor();
	this->cursor = LoadCursor(NULL, IDC_ARROW);
	this->cursorMove = false;
}

Input* Input::GetInstance() {
	if (Input::instance == nullptr)
		Input::instance = new Input();
	return Input::instance;
}

bool Input::GetKeyDown(char key) {
	return this->GetKey(key, PRESSED);
}

bool Input::GetKeyUp(char key) {
	return this->GetKey(key, RELEASED);
}

bool Input::GetKey(char key, EventType state) {
	int nKey = static_cast<int>(key);
	if (this->keyStates.count(nKey) > 0) {
		if (this->keyStates[nKey] == state)
			return true;
	}
	return false;
}

bool Input::GetMouseButton(MouseButton button, EventType state) {
	if (this->mouseButtons.count(button) > 0) {
		if (this->mouseButtons[button] == state)
			return true;
	}
	return false;
}

bool Input::GetMouseButtonDown(MouseButton button) {
	return this->GetMouseButton(button, PRESSED);
}

bool Input::GetMouseButtonUp(MouseButton button) {
	return this->GetMouseButton(button, RELEASED);
}

void Input::SetKeyState(char key, EventType state) {
	int nKey = static_cast<int>(key);
	if (this->keyStates.count(key) > 0) {
		if (this->keyStates[nKey] == state)
			return;
		
		std::map<int, EventType> keyCopy = this->keyStates;
		keyCopy[nKey] = state;
		
		this->keyStates.swap(keyCopy);
		return;
	}
	else {
		this->keyStates[nKey] = state;
	}
}

void Input::RemoveReleased() {
	if (this->keyStates.size() > 0) {
		std::vector<int> removedKeys;

		for (std::pair<int, EventType> key : this->keyStates) {
			if (key.second == RELEASED)
				removedKeys.push_back(key.first);
		}

		for (int key : removedKeys)
			this->keyStates.erase(key);
	}

	if (this->mouseButtons.size() > 0) {
		std::vector<MouseButton> removedButtons;

		for (std::pair<MouseButton, EventType> button : this->mouseButtons) {
			if (button.second == RELEASED)
				removedButtons.push_back(button.first);
		}

		for (MouseButton button : removedButtons)
			this->mouseButtons.erase(button);
	}
}

float Input::GetAxis(std::string axis) {
	if (axis == "Up")
		return this->deltaY;
	if (axis == "Right")
		return this->deltaX;
}

void Input::HideCursor(bool enabled) {
	this->cursorMove = enabled;
	if (enabled) {
		RECT rect;
		GetClientRect(this->hwnd, &rect);

		POINT ul;
		ul.x = rect.left;
		ul.y = rect.top;

		POINT lr;
		lr.x = rect.right;
		lr.y = rect.bottom;

		MapWindowPoints(this->hwnd, nullptr, &ul, 1);
		MapWindowPoints(this->hwnd, nullptr, &lr, 1);

		rect.left = ul.x;
		rect.top = ul.y;

		rect.right = lr.x;
		rect.bottom = lr.y;

		int width = rect.right - rect.left;
		int height = rect.bottom - rect.top;

		this->centerX = width / 2;
		this->centerY = height / 2;

		ClipCursor(&rect);
		SetCursor(NULL);
	}
	else {
		ClipCursor(NULL);
		SetCursor(this->cursor);
	}
}

void Input::SetMouseButtonState(MouseButton button, EventType state) {
	if (this->mouseButtons.count(button) > 0) {
		if (this->mouseButtons[button] == state)
			return;

		std::map<MouseButton, EventType> buttonCopy = this->mouseButtons;
		buttonCopy[button] = state;
		this->mouseButtons.swap(buttonCopy);
		return;
	}
	else
		this->mouseButtons[button] = state;
	return;
}

bool Input::TranslateInput(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (this->cursorMove) {
		GetCursorPos(&this->CursorPos);

		this->deltaX = this->centerX - this->CursorPos.x;
		this->deltaY = this->centerY - this->CursorPos.y;
		SetCursorPos(this->centerX, this->centerY);
	}

	BYTE keyboardState[256];
	bool keyboardStateStatus = GetKeyboardState(keyboardState);

	if (!keyboardStateStatus)
		return false;

	ToAscii(wParam, lParam, keyboardState, (LPWORD)&key, NULL);
}
