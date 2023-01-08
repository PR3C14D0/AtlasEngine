#include "Engine/Window.h"

bool g_quit = false;
Core* g_core = Core::GetInstance();
Input* g_input = Input::GetInstance();

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

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

	this->dbg = Debugger::GetInstance();
	this->time = Time::GetInstance();

	this->dbg->SetHWND(this->hwnd);
	g_core->SetHWND(this->hwnd);

	g_core->Init();
	g_input->Init();
	g_input->SetQuit(&g_quit);

	clock_t startTime = clock();
	float deltaTimeMs;
	clock_t endTime;

	/* Main message loop */
	MSG msg = { };
	while (!g_quit) {
		g_input->RemoveReleased();
		while (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		g_core->MainLoop();
		endTime = clock();
		deltaTimeMs = endTime - startTime;
		startTime = endTime;
		this->time->SetDelta(1.f / deltaTimeMs);
	}
}


LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
		return 0;

	g_input->TranslateInput(uMsg, wParam, lParam);

	switch (uMsg) {
	case WM_KEYDOWN:
		g_input->SetKeyState(g_input->key, PRESSED);
		return 0; 
	case WM_KEYUP:
		g_input->SetKeyState(g_input->key, RELEASED);
		return 0;
	case WM_LBUTTONDOWN:
		g_input->SetMouseButtonState(LEFT_CLICK, PRESSED);
		return 0;
	case WM_RBUTTONDOWN:
		g_input->SetMouseButtonState(RIGHT_CLICK, PRESSED);
		return 0;
	case WM_LBUTTONUP:
		g_input->SetMouseButtonState(LEFT_CLICK, RELEASED);
		return 0;
	case WM_RBUTTONUP:
		g_input->SetMouseButtonState(RIGHT_CLICK, RELEASED);
		return 0; 
	case WM_CLOSE:
		if (MessageBox(hwnd, "Are you sure you want to exit from Atlas engine?", "Sure?", MB_OKCANCEL) == IDOK) {
			DestroyWindow(hwnd);
		}
		return 0;
	case WM_DESTROY:
		g_quit = true;
		PostQuitMessage(0);
		return 0;
	}

	g_input->RemoveReleased();

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
};