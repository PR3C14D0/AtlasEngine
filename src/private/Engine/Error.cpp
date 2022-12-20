#include "Engine/Error.h"

Error* Error::instance;

Error* Error::GetInstance() {
	if (Error::instance == nullptr)
		Error::instance = new Error();
	return Error::instance;
}

void Error::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
	return;
}

void Error::Throw(std::string msg) {
	if (this->hwnd == NULL) {
		MessageBox(NULL, "[ERROR] No HWND set.\nFile: Error.h", "Error", MB_OK | MB_ICONERROR);
		return;
	}

	MessageBox(this->hwnd, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
}