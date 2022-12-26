#include "Engine/GameObject/Components/Mesh.h"
#include "Engine/Core.h"

Mesh::Mesh(Transform* transform) : Component::Component(transform) {
	this->name = "Mesh";
	this->core = Core::GetInstance();
	this->core->GetDevice(&this->dev, &this->con);

	/* Get our debugger singleton */
	this->dbg = Debugger::GetInstance();

	/* Log position info to the debugger */
	this->dbg->Log("[DEBUG] Creating a mesh component at: [X] " + std::to_string(this->transform->location.x) + " [Y] " + std::to_string(this->transform->location.y) + " [Z]" + std::to_string(this->transform->location.z));

	/* Get our constant buffer singleton */
	this->MVP = ConstantBuffer::GetInstance();

	/* Set our Model position at our MVP */
	this->MVP->Model = XMMatrixTranspose(XMMatrixIdentity() * XMMatrixTranslation(this->transform->location.x, this->transform->location.y, this->transform->location.z));

	this->ModelLoaded = false;
}

void Mesh::SetupBuffer() {
	/* Our buffer descriptor */
	D3D11_BUFFER_DESC buffDesc = { };
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

	buffDesc.ByteWidth = vertices.size() * sizeof(vertex);
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	this->dev->CreateBuffer(&buffDesc, nullptr, this->buff.GetAddressOf()); // Creation of our buffer

	/* Write to our buffer */
	D3D11_MAPPED_SUBRESOURCE ms;
	this->con->Map(this->buff.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, this->vertices.data(), sizeof(vertex) * this->vertices.size());
	this->con->Unmap(this->buff.Get(), NULL);

	/* Shader compilation */
	ID3DBlob* VertexShader, *PixelShader;
	D3DX11CompileFromFile("GameObject.fx", nullptr, NULL, "VertexMain", "vs_4_0", NULL, NULL, nullptr, &VertexShader, nullptr, nullptr);
	D3DX11CompileFromFile("GameObject.fx", nullptr, NULL, "PixelMain", "ps_4_0", NULL, NULL, nullptr, &PixelShader, nullptr, nullptr);

	/* Creation of our input layout */
	ComPtr<ID3D11InputLayout> iLayout;

	D3D11_INPUT_ELEMENT_DESC iLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL},
	};

	this->dev->CreateInputLayout(iLayoutDesc, 2, VertexShader->GetBufferPointer(), VertexShader->GetBufferSize(), iLayout.GetAddressOf());

	/* Create out shaders */
	ComPtr<ID3D11VertexShader> vShader;
	ComPtr<ID3D11PixelShader> pShader;

	this->dev->CreateVertexShader(VertexShader->GetBufferPointer(), VertexShader->GetBufferSize(), nullptr, vShader.GetAddressOf());
	this->dev->CreatePixelShader(PixelShader->GetBufferPointer(), PixelShader->GetBufferSize(), nullptr, pShader.GetAddressOf());

	/* Use our shaders */
	this->con->VSSetShader(vShader.Get(), nullptr, NULL);
	this->con->PSSetShader(pShader.Get(), nullptr, NULL);

	/* Use our input layout */
	this->con->IASetInputLayout(iLayout.Get());
}

void Mesh::SetupCBuffer() {
	/* Our constant buffer descriptor */
	D3D11_BUFFER_DESC buffDesc = { };
	ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

	buffDesc.ByteWidth = sizeof(*this->MVP);
	buffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	buffDesc.Usage = D3D11_USAGE_DYNAMIC;
	buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	this->dev->CreateBuffer(&buffDesc, nullptr, this->CBuff.GetAddressOf()); // Our buffer creation

	this->UpdateCBuffer();
	this->con->VSSetConstantBuffers(0, 1, this->CBuff.GetAddressOf());
}

void Mesh::PreRender() {
	Component::PreRender();

	if (this->ModelLoaded) {
		this->SetupCBuffer();
		this->SetupBuffer();
	}
}

void Mesh::UpdateCBuffer() {
	D3D11_MAPPED_SUBRESOURCE ms;
	this->con->Map(this->CBuff.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
	memcpy(ms.pData, this->MVP, sizeof(*MVP));
	this->con->Unmap(this->CBuff.Get(), NULL);
}

void Mesh::Update() {
	Component::Update();

	if (this->ModelLoaded) {
		this->UpdateCBuffer();
		UINT offset = 0;
		UINT stride = sizeof(vertex);
		/* We'll print triangles */
		this->con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->con->IASetVertexBuffers(0, 1, this->buff.GetAddressOf(), &stride, &offset);

		/* The magic method. The ~DRAW~ */
		this->con->Draw(this->vertices.size(), 0);
	}

}

void Mesh::LoadModel(std::string name) {
	/* We'll use assimp for model loading */
	this->ModelLoaded = true;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(name, NULL);

	if (scene->mNumMeshes > 0) {
		aiMesh* mesh = scene->mMeshes[0];
		aiMaterial* mat = scene->mMaterials[mesh->mMaterialIndex];

		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D pos = mesh->mVertices[i];
			vertex vert = { pos.x, pos.y, pos.z, {mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y} };
			this->vertices.push_back(vert);

		}

		aiString texPath;
		if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &texPath) == AI_SUCCESS) {
			HRESULT hr = D3DX11CreateShaderResourceViewFromFile(this->dev.Get(), texPath.C_Str(), nullptr, nullptr, this->modelTex.GetAddressOf(), nullptr);

			if (FAILED(hr)) {
				dbg->Throw("[ERROR] An error occurred while creating shader resource view for a model.\nFile: Mesh.cpp\nMethod: D3DX11CreateShaderResourceViewFromFile");
				std::cout << "[ERROR] An error occurred while creating shader resource view for a model.\nFile: Mesh.cpp\nMethod: D3DX11CreateShaderResourceViewFromFile" << std::endl;
			}

			D3D11_SAMPLER_DESC samplerDesc = { };
			ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.MinLOD = 0.f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
			samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
				
			this->dev->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());
			this->con->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
			this->con->PSSetShaderResources(0, 1, this->modelTex.GetAddressOf());
		}
	}
}

void Mesh::Cleanup() {
	this->ModelLoaded = false;
	this->buff->Release();
	this->modelTex->Release();
	this->samplerState->Release();
	this->CBuff->Release();
}