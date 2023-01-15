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
#include "Module/Util.h"
#include "Engine/Shader.h"
#include "Module/Time.h"
#include <execution>
#include <map>

using namespace Microsoft::WRL;

class Core; /*  Forward declaration of our Core class for preventing circular includes */

class Mesh : public Component {
private:
	Core* core;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;

	ComPtr<ID3D11Buffer> CBuff;

	ComPtr<ID3D11SamplerState> samplerState;
	std::map<UINT, ComPtr<ID3D11ShaderResourceView>> textures;

	std::map<UINT, std::vector<vertex>> vertices;
	std::map<UINT, ComPtr<ID3D11Buffer>> buffers;
	ConstantBuffer* MVP;
	void SetupBuffers();
	void SetupCBuffer();

	void UpdateCBuffer();

	void UseCBuffer();

	UINT MeshCount;

	Debugger* dbg;

	bool ModelLoaded;

	Shader* shader;

	Time* time;
public:
	Mesh(Transform* transform);

	void PreRender() override;
	void Update() override;
	void LoadModel(std::string name) override;

	void Cleanup() override;
};