#pragma once
#include <iostream>
#include <string>
#include "Engine/GameObject/Component.h"
#include <DirectX/DXGI.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include <DirectX/xnamath.h>
#include "Engine/ConstantBuffer.h"
#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>
#include "Engine/Vertex.h"
#include <vector>
#include <wrl.h>
#include "Engine/Debugger.h"
#include <ImGui/imgui.h>
#include <ImGui/backends/imgui_impl_win32.h>
#include <ImGui/backends/imgui_impl_dx11.h>

using namespace Microsoft::WRL;

class Core; /*  Forward declaration of our Core class for preventing circular includes */

class Mesh : public Component {
private:
	Core* core;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;

	ComPtr<ID3D11Buffer> buff;
	ComPtr<ID3D11Buffer> CBuff;

	ComPtr<ID3D11ShaderResourceView> modelTex;
	ComPtr<ID3D11SamplerState> samplerState;

	std::vector<vertex> vertices;
	ConstantBuffer* MVP;
	void SetupBuffer();
	void SetupCBuffer();

	void UpdateCBuffer();

	Debugger* dbg;

	bool ModelLoaded;
public:
	Mesh(Transform* transform);

	void PreRender() override;
	void Update() override;
	void LoadModel(std::string name) override;

	void Cleanup() override;
};