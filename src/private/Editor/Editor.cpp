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

	this->dbg = Debugger::GetInstance();

	/* Init ImGUI context and IO */
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.Fonts->AddFontDefault();

	ImGui::StyleColorsDark();

	/* Init ImGUI backends */
	ImGui_ImplWin32_Init(this->hwnd);
	ImGui_ImplDX11_Init(this->dev.Get(), this->con.Get()); 
	
	this->scene->GetObjects(&this->objects);
	this->ObjectPropertiesOpen = false;
	this->workingObject = nullptr;

	this->time = Time::GetInstance();
	this->deltaTime = time->deltaTime;
}

Editor* Editor::GetInstance() {
	if (Editor::instance == nullptr)
		Editor::instance = new Editor();
	return Editor::instance;
}

void Editor::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
}

void Editor::Debugger() {
	ImGui::SetNextWindowSize(ImVec2{ 840.f, 150.f });
	ImGui::SetNextWindowPos(ImVec2{ 280.f, 520.f });
	ImGui::Begin("Debugger", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	for (std::string msg : this->dbgMessages) {
		ImVec4 msgColor = ImColor{ 0.f, 0.f, 0.f };
		if (msg.starts_with("[ERROR]")) {
			msgColor = ImColor{ 255.f, 0.f, 0.f };
		}

		if (msg.starts_with("[DEBUG]")) {
			msgColor = ImColor{ 255.f, 255.f, 0.f };
		}

		ImGui::PushStyleColor(ImGuiCol_Text, msgColor);
		ImGui::Text(msg.c_str());
		ImGui::PopStyleColor();
	}
	ImGui::End();
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

void Editor::ObjectProperties() {
	if (this->ObjectPropertiesOpen && (this->workingObject != nullptr)) {
		ImGui::SetNextWindowSize(ImVec2{ 200.f, 600.f });
		ImGui::SetNextWindowPos(ImVec2{ 1150.f, 60.f });
		ImGui::Begin("Properties", &this->ObjectPropertiesOpen, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::InputText("Name ##ObjectName", &this->workingObject->name, 16);
		ImGui::End();
	}
}

void Editor::Hierarchy() {
	ImGui::SetNextWindowSize(ImVec2{ 200.f, 600.f });
	ImGui::SetNextWindowPos(ImVec2{ 60.f, 60.f });
	ImGui::Begin("Hierarchy", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	for (GameObject* object : objects) {
		if (ImGui::Button(object->name.c_str())) {
			if (this->ObjectPropertiesOpen && this->workingObject == object) {
				this->ObjectPropertiesOpen = false;
			}
			else {
				this->ObjectPropertiesOpen = true;
			}
			this->workingObject = object;
		}
	}
	ImGui::End();
}

void Editor::Performance() {
	ImGui::SetNextWindowPos(ImVec2{ 1020.f, 80.f });
	ImGui::SetNextWindowSize(ImVec2{ 120.f, 50.f });
	ImGui::Begin("Performance", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	ImGui::Text(("FPS: " + std::to_string((int)ceil(1000 * deltaTime))).c_str());
	ImGui::End();
}

void Editor::Update() {
	this->dbg->GetMessages(&this->dbgMessages);
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	
	this->Debugger();
	this->MenuBar();
	this->Hierarchy();
	this->ObjectProperties();
	this->Performance();

	this->deltaTime = this->time->deltaTime;

	ImGui::Render();
}