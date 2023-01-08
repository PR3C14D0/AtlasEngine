#pragma once
#include "Engine/Debugger.h"
#include <Windows.h>
#include <wrl.h>
#include <DirectX/DXGI.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include "Engine/Scene/SceneManager.h"
#include "Engine/Vertex.h"
#include "Editor/Editor.h"

using namespace Microsoft::WRL;

enum VSYNC {
	DISABLED = 0,
	ENABLED = 1,
	MEDIUM = 2
};

class Core {
	friend class SceneManager;
private:
	static Core* instance;
	HWND hwnd;

	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;
	ComPtr<IDXGISwapChain> sc;

	ComPtr<ID3D11RenderTargetView> backBuffer;
	ComPtr<ID3D11DepthStencilView> depthBuffer;

	Editor* editor;

	int width, height;

	SceneManager* sceneMgr;

	VSYNC vSyncState;

	Debugger* dbg;
public:
	Core() = default;;
	static Core* GetInstance();

	void Init();
	void MainLoop();
	
	void GetHWND(HWND* pHWND);

	void SetHWND(HWND& hwnd);
	void GetDevice(ComPtr<ID3D11Device>* pDev, ComPtr<ID3D11DeviceContext>* pCon);

	void SetVSyncState(VSYNC state);

	void Shutdown();

	SceneManager* GetSceneManager();
};