#include "Editor/Editor.h"
#include "Engine/Core.h"

Editor* Editor::instance;

Editor::Editor() {
	/* Get our core singleton, hwnd, device and inmediate context */
	this->core = Core::GetInstance();
	this->core->GetDevice(&this->dev, &this->con);
	this->core->GetHWND(&this->hwnd);

	/* Get our scene manager and actual scene */
	this->sceneMgr = this->core->GetSceneManager();
	this->scene = this->sceneMgr->GetActualScene();

	/* Init ImGUI context and IO */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();

	/* Init ImGUI backends */
	ImGui_ImplWin32_Init(this->hwnd);
	ImGui_ImplDX11_Init(this->dev.Get(), this->con.Get());
}

Editor* Editor::GetInstance() {
	if (Editor::instance == nullptr)
		Editor::instance = new Editor();
	return Editor::instance;
}

void Editor::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
}

void Editor::MenuBar() {
	ImGui::BeginMainMenuBar();
	ImGui::MenuItem("File");
	ImGui::MenuItem("Edit");
	ImGui::MenuItem("Settings");
	if (ImGui::MenuItem("Exit")) {
		if (MessageBox(this->hwnd, "Are you sure you want to exit from Atlas engine?", "Sure?", MB_OKCANCEL) == IDOK)
			DestroyWindow(hwnd);
	}
	
	ImGui::EndMainMenuBar();
}

void Editor::Hierarchy() {
	ImGui::SetNextWindowSize(ImVec2{ 200.f, 600.f });
	ImGui::Begin("Hierarchy");
	ImGui::End();
}

void Editor::Update() {
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	this->MenuBar();
	this->Hierarchy();

	ImGui::Render();
}