#pragma once
#include <Windows.h>
#include <wrl.h>
#include <DirectX/DXGI.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include "Engine/Error.h"
#include "Engine/Scene/SceneManager.h"

using namespace Microsoft::WRL;

typedef float RGBA[4];

struct vertex {
	float x, y, z;
	RGBA color;
};

class Core {
private:
	static Core* instance;
	HWND hwnd;

	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;
	ComPtr<IDXGISwapChain> sc;

	ComPtr<ID3D11RenderTargetView> backBuffer;

	ComPtr<ID3D11Buffer> buff;

	int width, height;

	SceneManager* sceneMgr;

	Error* error;
public:
	Core() = default;;
	static Core* GetInstance();

	void Init();
	void MainLoop();

	void SetHWND(HWND& hwnd);
};