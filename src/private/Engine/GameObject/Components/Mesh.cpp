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

	/* Get our constant buffer singleton and set our MVP */
	/* TODO: Get View and Projection from actual camera and stop using MVP singleton */

	ConstantBuffer* pMVP = ConstantBuffer::GetInstance();
	this->MVP = pMVP;

	/* Set our Model position at our MVP */
	this->MVP->Model = XMMatrixTranspose(XMMatrixIdentity() * XMMatrixTranslation(this->transform->location.x, this->transform->location.y, this->transform->location.z));

	this->time = Time::GetInstance();

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

	/* Creation of our shader */
	D3D11_INPUT_ELEMENT_DESC iLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL}
	};

	UINT nNumElements = sizeof(iLayoutDesc) / sizeof(D3D11_INPUT_ELEMENT_DESC);
	
	this->shader = new Shader("GameObject.fx", "VertexMain", "PixelMain", iLayoutDesc, nNumElements);
	this->shader->UseShader();
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
	this->con->PSSetConstantBuffers(0, 1, this->CBuff.GetAddressOf());
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

	this->MVP->Model = XMMatrixTranspose(XMMatrixIdentity() * XMMatrixTranslation(this->transform->location.x, this->transform->location.y, this->transform->location.z));

	if (this->ModelLoaded) {
		this->UpdateCBuffer();
		UINT offset = 0;
		UINT stride = sizeof(vertex);

		/* We'll print triangles */
		this->con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		this->con->IASetVertexBuffers(0, 1, this->buff.GetAddressOf(), &stride, &offset);

		/* The magic method. The draw method */
		this->con->Draw(this->vertices.size(), 0);
	}

}

void Mesh::LoadModel(std::string name) {
	/* We'll use assimp for model loading */
	this->ModelLoaded = true;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(name, NULL);
	
	if (scene->HasMeshes()) { /* Check if our scene has meshes */
		std::vector<aiMesh*> meshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
		aiMesh* mesh = meshes[0];
		aiMaterial* mat = nullptr;

		if (mesh->HasTextureCoords(0)) {
			mat = scene->mMaterials[mesh->mMaterialIndex];
		}

		for (int i = 0; i < mesh->mNumVertices; i++) {
			aiVector3D aiPos = mesh->mVertices[i];
			position pos = { aiPos.x, aiPos.y, aiPos.z };
			tCoord texCoords = { 0.f, 0.f };
			normals nml = { 0.f, 0.f, 0.f };

			if (mesh->HasTextureCoords(0)) {
				aiVector3D aiTexCoords = mesh->mTextureCoords[0][i];
				texCoords[0] = aiTexCoords.x;
				texCoords[1] = aiTexCoords.y;
			}

			if (mesh->HasNormals()) {
				nml[0] = mesh->mNormals[i].x;
				nml[1] = mesh->mNormals[i].y;
				nml[2] = mesh->mNormals[i].z;
			}

			vertex pushedVertex = { { pos[0], pos[1], pos[2] }, { texCoords[0], texCoords[1] }, { nml[0], nml[1], nml[2]}};
			this->vertices.push_back(pushedVertex);
		}

		if (mat) {
			aiString texturePath;
			if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS) {
				const aiTexture* texture = scene->GetEmbeddedTexture(texturePath.C_Str());
				D3DX11CreateShaderResourceViewFromMemory(this->dev.Get(), texture->pcData, texture->mWidth, nullptr, nullptr, this->modelTex.GetAddressOf(), nullptr);
			}

			D3D11_SAMPLER_DESC samplerDesc = { };
			ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

			samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
			samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
			samplerDesc.MinLOD = 0.f;
			samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

			this->dev->CreateSamplerState(&samplerDesc, this->samplerState.GetAddressOf());

			this->con->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
			this->con->PSSetShaderResources(0, 1, this->modelTex.GetAddressOf());
		}
	}
}

void Mesh::Cleanup() {
	this->ModelLoaded = false;
	if(this->buff)
		this->buff->Release();

	if(this->modelTex)
		this->modelTex->Release();

	if(this->samplerState)
		this->samplerState->Release();

	if(this->CBuff)
		this->CBuff->Release();
}