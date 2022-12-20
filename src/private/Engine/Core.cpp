#include "Engine/Core.h"

Core* Core::instance;

void Core::Init() {
	this->error = Error::GetInstance();
	if (this->hwnd == NULL) {
		error->Throw("[ERROR] Core has no window setten.\nFile: Core.cpp");
		std::cout << "[ERROR] Core has no window setten.\nFile: Core.cpp" << std::endl;
		exit(0);
	}


	/* Our SwapChain descriptor */
	DXGI_SWAP_CHAIN_DESC scDesc = { };
	ZeroMemory(&scDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	scDesc.BufferCount = 1; // We only want to create the backbuffer
	scDesc.Windowed = TRUE;
	scDesc.SampleDesc.Count = 4;
	scDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.OutputWindow = this->hwnd;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	/* Creation of our Device, Inmediate context and Swap chain */
	HRESULT hr = D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		NULL,
		nullptr,
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
		error->Throw("[ERROR] An error occurred while creating device, inmediate context and swap chain\nFile: Core.cpp\nMethod: D3D11CreateDeviceAndSwapChain");
		std::cout << "[ERROR] An error occurred while creating device, inmediate context and swap chain\nFile: Core.cpp\nMethod: D3D11CreateDeviceAndSwapChain" << std::endl;
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

	this->con->RSSetViewports(1, &viewport); // Set our viewport as actual viewport
	this->con->OMSetRenderTargets(1, this->backBuffer.GetAddressOf(), nullptr); // Set out backbuffer as actual backbuffer

	this->sceneMgr = new SceneManager();
}

void Core::MainLoop() {
	this->con->ClearRenderTargetView(this->backBuffer.Get(), RGBA{ 0.f, 0.f, 0.f, 1.f }); // Clear our backbuffer
	this->sceneMgr->Update();
	this->sc->Present(1, 0); // Present our backbuffer
}

void Core::SetHWND(HWND& hwnd) {
	this->hwnd = hwnd;
}

void Core::GetDevice(ComPtr<ID3D11Device>* pDev, ComPtr<ID3D11DeviceContext>* pCon)
{
	*pDev = this->dev;
	*pCon = this->con;
}

Core* Core::GetInstance() {
	if (Core::instance == nullptr)
		Core::instance = new Core();
	return Core::instance;
}