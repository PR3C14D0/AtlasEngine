#pragma once
#include "Engine/GameObject/Component.h"
#include <DirectX/DXGI.h>
#include <DirectX/D3D11.h>
#include <DirectX/D3DX11.h>
#include "Engine/Vertex.h"
#include <vector>
#include <wrl.h>

using namespace Microsoft::WRL;

class Core;

class Mesh : public Component {
private:
	Core* core;
	ComPtr<ID3D11Device> dev;
	ComPtr<ID3D11DeviceContext> con;

	ComPtr<ID3D11Buffer> buff;

	std::vector<vertex> vertices;
public:
	Mesh(Transform* transform);

	void PreRender() override;
	void Update() override;
	void LoadModel() override;
	void SetupBuffer();
};