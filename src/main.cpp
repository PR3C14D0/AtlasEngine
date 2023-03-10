#include "Engine/Window.h"

void EnableConsole();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
#ifndef NDEBUG
	EnableConsole();
#endif

	std::cout << "Atlas engine debugger console." << std::endl;

	Window* window = new Window(hInstance, hPrevInstance, lpCmdLine, nShowCmd);

	return 0;
}

void EnableConsole() {
	AllocConsole();
	FILE* f;
	freopen_s(&f, "CONOUT$", "w", stdout);
}