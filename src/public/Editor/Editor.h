#pragma once
#include <Windows.h>
#include <wrl.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_dx11.h>
#include <ImGui/backends/imgui_impl_win32.h>

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

	Core* core;
	SceneManager* sceneMgr;
	Scene* scene;

	HWND hwnd;

	void MenuBar();
	void Hierarchy();
public:
	static Editor* GetInstance();

	void SetHWND(HWND& hwnd);

	void Update();
};