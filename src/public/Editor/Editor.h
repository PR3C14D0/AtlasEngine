#pragma once
#include <iostream>
#include <Windows.h>
#include <wrl.h>
#include "Engine/GameObject/GameObject.h"
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_dx11.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <vector>
#include <ImGui/misc/cpp/imgui_stdlib.h>
#include "Engine/Debugger.h"
#include "Module/Time.h"

using namespace Microsoft::WRL;

/* Start:Forward declarations */
class Core;
class SceneManager;
class Scene;
/* End:Forward declarations */

class Editor {
private:
	Editor();
	static Editor* instance;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;

	Debugger* dbg;

	Core* core;
	SceneManager* sceneMgr;
	Scene* scene;

	HWND hwnd;

	std::vector<GameObject*> objects;
	std::vector<std::string> dbgMessages;

	void MenuBar();
	void Hierarchy();

	bool ObjectPropertiesOpen;
	void ObjectProperties();
	void Debugger();
	void Performance();

	float deltaTime;

	Time* time;

	GameObject* workingObject;
public:
	static Editor* GetInstance();

	void SetHWND(HWND& hwnd);

	void Update();
};