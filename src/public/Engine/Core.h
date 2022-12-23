#pragma once
#include "Engine/Error.h"
#include <Windows.h>
#include <wrl.h>
#include <DirectX/DXGI.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include "Engine/Scene/SceneManager.h"
#include "Engine/Vertex.h"

using namespace Microsoft::WRL;

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

	int width, height;

	SceneManager* sceneMgr;

	Error* error;
public:
	Core() = default;;
	static Core* GetInstance();

	ComPtr<ID3D11Buffer> buff;
	void Init();
	void MainLoop();

	void SetHWND(HWND& hwnd);
	void GetDevice(ComPtr<ID3D11Device>* pDev, ComPtr<ID3D11DeviceContext>* pCon);
};