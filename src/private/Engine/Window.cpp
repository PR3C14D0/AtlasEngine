#include "Engine/Window.h"

bool g_quit = false;

Window::Window(HINSTANCE& hInstance, HINSTANCE& hPrevInstance, LPSTR& lpCmdLine, int& nShowCmd) {
	const char* CLASS_NAME = "AtlasEngine"; // Our window class name

	/* Window class initialization */
	WNDCLASS wc = { };
	wc.hInstance = hInstance;
	wc.lpszClassName = CLASS_NAME;
	wc.lpfnWndProc = this->WindowProc;

	RegisterClass(&wc); // Register our window class

	/* Our window creation */
	this->hwnd = CreateWindowEx(
		NULL,
		CLASS_NAME,
		"Atlas Engine",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);

	/* Check if our window exists */
	if (this->hwnd == NULL) {
		MessageBox(NULL, "[ERROR] An error occurred while creating window.\nFile: Window.cpp\nMethod: CreateWindowEx", "Error", MB_OK | MB_ICONERROR);
		exit(0);
	}

	ShowWindow(this->hwnd, nShowCmd); // Show our window

	this->error = Error::GetInstance();
	this->core = Core::GetInstance();

	this->error->SetHWND(this->hwnd);
	this->core->SetHWND(this->hwnd);

	this->core->Init();

	/* Main message loop */
	MSG msg = { };
	while (!g_quit) {
		if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		this->core->MainLoop();
	}
}

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
		if (MessageBox(hwnd, "Are you sure you want to exit from Atlas engine?", "Sure?", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		g_quit = true;
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};