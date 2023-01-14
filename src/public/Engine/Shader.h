#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include <wrl.h>
#include "Engine/Debugger.h"

using namespace Microsoft::WRL;

class Core;

class Shader {
private:
	ComPtr<ID3D11VertexShader> VShader;
	ComPtr<ID3D11PixelShader> PShader;

	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;

	ComPtr<ID3D11InputLayout> iLayout;

	Debugger* dbg;

	Core* core;
public:
	Shader(std::string path, std::string sVertexShaderFuncName, std::string sPixelShaderFuncName, const D3D11_INPUT_ELEMENT_DESC* iLayoutDesc, UINT nNumElements);
	~Shader();

	void UseShader();
};