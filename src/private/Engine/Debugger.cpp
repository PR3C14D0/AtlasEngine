#include "Engine/Debugger.h"

Debugger* Debugger::instance;

Debugger* Debugger::GetInstance() {
	if (Debugger::instance == nullptr)
		Debugger::instance = new Debugger();
	return Debugger::instance;
}

void Debugger::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
	return;
}

void Debugger::GetMessages(std::vector<std::string>* msgs) {
	*msgs = this->dbgMsgs;
}

void Debugger::Throw(std::string msg) {
	if (this->hwnd == NULL) {
		std::string hwndError = "[ERROR] No HWND set.\nFile: Error.h";
		MessageBox(NULL, hwndError.c_str(), "Error", MB_OK | MB_ICONERROR);
		std::cout << hwndError << std::endl;
		return;
	}

	MessageBox(this->hwnd, msg.c_str(), "Error", MB_OK | MB_ICONERROR);
	std::cout << msg << std::endl;
}

void Debugger::Log(std::string msg) {
	if (this->hwnd == NULL) {
		std::string hwndError = "[ERROR] No HWND set.\nFile: Error.h";
		MessageBox(NULL, hwndError.c_str(), "Error", MB_OK | MB_ICONERROR);
		std::cout << hwndError << std::endl;
		return;
	}

	this->dbgMsgs.push_back(msg);
	std::cout << msg << std::endl;
}