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

void Mesh::SetupBuffers() {
	for (std::pair<UINT,std::vector<vertex>> vertice : this->vertices) {
		std::vector<vertex> vert = vertice.second;

		ComPtr <ID3D11Buffer> buffer;

		/* Our buffer descriptor */
		D3D11_BUFFER_DESC buffDesc = { };
		ZeroMemory(&buffDesc, sizeof(D3D11_BUFFER_DESC));

		buffDesc.ByteWidth = vert.size() * sizeof(vertex);
		buffDesc.Usage = D3D11_USAGE_DYNAMIC;
		buffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		buffDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		this->dev->CreateBuffer(&buffDesc, nullptr, buffer.GetAddressOf()); // Creation of our buffer

		/* Write to our buffer */
		D3D11_MAPPED_SUBRESOURCE ms;
		this->con->Map(buffer.Get(), NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);
		memcpy(ms.pData, vert.data(), sizeof(vertex) * vert.size());
		this->con->Unmap(buffer.Get(), NULL);

		this->buffers[vertice.first] = buffer;
	}

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
	this->UseCBuffer();
}

void Mesh::UseCBuffer() {
	this->con->VSSetConstantBuffers(0, 1, this->CBuff.GetAddressOf());
	this->con->PSSetConstantBuffers(0, 1, this->CBuff.GetAddressOf());
}

void Mesh::PreRender() {
	Component::PreRender();

	if (this->ModelLoaded) {
		this->SetupCBuffer();
		this->SetupBuffers();
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
		this->UseCBuffer();
		this->UpdateCBuffer();
		this->shader->UseShader();
		this->con->PSSetSamplers(0, 1, this->samplerState.GetAddressOf());
		for (int i = 0; i < this->buffers.size(); i++) {
			UINT offset = 0;
			UINT stride = sizeof(vertex);

			/* We'll print triangles */
			this->con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			this->con->IASetVertexBuffers(0, 1, this->buffers[i].GetAddressOf(), &stride, &offset); //

			this->con->PSSetShaderResources(0, 1, this->textures[i].GetAddressOf());

			/* The magic method. The draw method */
			this->con->Draw(this->vertices[i].size(), 0);
		}
	}

}

void Mesh::LoadModel(std::string name) {
	/* We'll use assimp for model loading */
	this->ModelLoaded = true;

	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(name, NULL);
	
	if (scene->HasMeshes()) { /* Check if our scene has meshes */
		std::vector<aiMesh*> meshes(scene->mMeshes, scene->mMeshes + scene->mNumMeshes);
		this->MeshCount = meshes.size();
		
		for (int index = 0; index < meshes.size(); index++) {
			aiMesh* mesh = meshes[index];
			aiMaterial* mat = nullptr;


			if (mesh->HasTextureCoords(0)) {
				mat = scene->mMaterials[mesh->mMaterialIndex];
			}

			for (int i = 0; i < mesh->mNumVertices; i++) {
				position pos = { 0.f, 0.f, 0.f };
				tCoord texCoord = { 0.f, 0.f };
				normals normal = { 0.f, 0.f, 0.f };

				aiVector3D aiPos = mesh->mVertices[i];
				pos[0] = aiPos.x;
				pos[1] = aiPos.y;
				pos[2] = aiPos.z;

				if (mesh->HasTextureCoords(0)) {
					texCoord[0] = mesh->mTextureCoords[0][i].x;
					texCoord[1] = mesh->mTextureCoords[0][i].y;
				}

				if (mesh->HasNormals()) {
					normal[0] = mesh->mNormals[i].x;
					normal[1] = mesh->mNormals[i].y;
					normal[2] = mesh->mNormals[i].z;
				}

				vertex pushedVertex = { { pos[0], pos[1], pos[2] }, { texCoord[0], texCoord[1] }, { normal[0], normal[1], normal[2] } };
				this->vertices[index].push_back(pushedVertex);
			}

			if (mat) {
				aiString path;
				if (mat->GetTextureCount(aiTextureType_DIFFUSE) > 0 && mat->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					const aiTexture* tex = scene->GetEmbeddedTexture(path.C_Str());

					D3DX11CreateShaderResourceViewFromMemory(this->dev.Get(), tex->pcData, tex->mWidth, nullptr, nullptr, this->textures[index].GetAddressOf(), nullptr);
				}
			}
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
	}
}

void Mesh::Cleanup() {
	/*this->ModelLoaded = false;
	if(this->buff)
		this->buff->Release();

	if(this->modelTex)
		this->modelTex->Release();

	if(this->samplerState)
		this->samplerState->Release();

	if(this->CBuff)
		this->CBuff->Release();*/
}