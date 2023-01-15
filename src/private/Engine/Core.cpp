#include "Engine/Core.h"

Core* Core::instance;

void Core::Init() {
	this->vSyncState = VSYNC::ENABLED;
	if (this->hwnd == NULL) {
		dbg->Throw("[ERROR] Core has no window setten.\nFile: Core.cpp");
		std::cout << "[ERROR] Core has no window setten.\nFile: Core.cpp" << std::endl;
		exit(0);
	}

	/* Our SwapChain descriptor */
	DXGI_SWAP_CHAIN_DESC scDesc = { };
	ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	scDesc.BufferCount = 1; // We only want to create the backbuffer
	scDesc.Windowed = TRUE;
	scDesc.SampleDesc.Count = 8;
	scDesc.BufferDesc.Format = DXGI_FORMAT_R10G10B10A2_UNORM;
	scDesc.OutputWindow = this->hwnd;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	D3D_FEATURE_LEVEL featureLevel[] = {
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_11_0,
	};

	/* Creation of our Device, Inmediate context and Swap chain */
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&scDesc,
		this->sc.GetAddressOf(),
		this->dev.GetAddressOf(),
		nullptr,
		this->con.GetAddressOf()
	);

	/* Verify if D3D11CreateDeviceAndSwapChain method didn't failed */
	if (FAILED(hr)) {
		dbg->Throw("[ERROR] An error occurred while creating device, inmediate context and swap chain\nFile: Core.cpp\nMethod: D3D11CreateDeviceAndSwapChain");
		DestroyWindow(this->hwnd);
	}

	/* Creation of our backbuffer */
	ComPtr<ID3D11Texture2D> pBackBuffer;
	this->sc->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.GetAddressOf());

	this->dev->CreateRenderTargetView(pBackBuffer.Get(), nullptr, this->backBuffer.GetAddressOf());
	pBackBuffer->Release();

	/* Get our window size */
	RECT rect;
	GetWindowRect(this->hwnd, &rect);
	this->width = rect.right - rect.left;
	this->height = rect.bottom - rect.top;
	
	/* Creation of our viewport */
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Height = this->height;
	viewport.Width = this->width;
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	ComPtr<ID3D11Texture2D> depthTex; // Our depth buffer texture;

	/* Our depth buffer texture descriptor */
	D3D11_TEXTURE2D_DESC depthTexDesc = { };
	ZeroMemory(&depthTexDesc, sizeof(D3D11_TEXTURE2D_DESC));

	depthTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthTexDesc.ArraySize = 1;
	depthTexDesc.MipLevels = 1;
	depthTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthTexDesc.Height = this->height;
	depthTexDesc.Width = this->width;
	depthTexDesc.SampleDesc.Count = 8;

	this->dev->CreateTexture2D(&depthTexDesc, nullptr, depthTex.GetAddressOf()); // Creation of our depth buffer texture

	/* Our DSV (Depth stencil view) descriptor */
	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = { };
	ZeroMemory(&dsvDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	dsvDesc.Format = depthTexDesc.Format;
	dsvDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	this->dev->CreateDepthStencilView(depthTex.Get(), &dsvDesc, this->depthBuffer.GetAddressOf()); // Our depth stencil view creation
	depthTex->Release();

	D3D11_RASTERIZER_DESC rDesc = { };
	ZeroMemory(&rDesc, sizeof(D3D11_RASTERIZER_DESC));

	rDesc.CullMode = D3D11_CULL_BACK;
	rDesc.AntialiasedLineEnable = TRUE;
	rDesc.FillMode = D3D11_FILL_SOLID;
	rDesc.MultisampleEnable = TRUE;

	ComPtr<ID3D11RasterizerState> rasterizer;
	this->dev->CreateRasterizerState(&rDesc, rasterizer.GetAddressOf());
	
	this->con->RSSetState(rasterizer.Get());
	this->con->RSSetViewports(1, &viewport); // Set our viewport as actual viewport
	this->con->OMSetRenderTargets(1, this->backBuffer.GetAddressOf(), this->depthBuffer.Get()); // Set out backbuffer and depthbuffer as render targets

	this->sceneMgr = new SceneManager();
	this->editor = Editor::GetInstance();
	this->dbg = Debugger::GetInstance();
}

void Core::MainLoop() {
	this->con->ClearRenderTargetView(this->backBuffer.Get(), RGBA{ 0.f, 0.f, 0.f, 1.f }); // Clear our backbuffer
	this->con->ClearDepthStencilView(this->depthBuffer.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0.f); // Clear out DepthBuffer
	this->editor->Update();
	this->sceneMgr->Update();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	this->sc->Present(this->vSyncState, 0); // Present our backbuffer
}

void Core::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
}

void Core::GetHWND(HWND* pHWND) {
	*pHWND = this->hwnd;
}

void Core::GetDevice(ComPtr<ID3D11Device>* pDev, ComPtr<ID3D11DeviceContext>* pCon)
{
	this->dev.CopyTo(pDev->GetAddressOf());
	this->con.CopyTo(pCon->GetAddressOf());
}

void Core::SetVSyncState(VSYNC state) {
	this->vSyncState = state;
}

void Core::Shutdown() {
	this->backBuffer->Release();
	this->depthBuffer->Release();
	this->dev->Release();
	this->con->Release();
	this->sceneMgr->GetActualScene()->ClearObjects();
}

SceneManager* Core::GetSceneManager() {
	return this->sceneMgr;
}

Core* Core::GetInstance() {
	if (Core::instance == nullptr)
		Core::instance = new Core();
	return Core::instance;
}