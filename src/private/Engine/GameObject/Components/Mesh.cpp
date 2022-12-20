#include "Engine/GameObject/Components/Mesh.h"
#include "Engine/Core.h"

Mesh::Mesh(Transform* transform) : Component::Component(transform) {
	this->name = "Mesh";
	this->core = Core::GetInstance();
	this->core->GetDevice(&this->dev, &this->con);
}

void Mesh::SetupBuffer() {
	this->vertices.push_back({ 0.f, 1.f, 0.f, {1.f, 0.f, 0.f, 1.f} });
	this->vertices.push_back({ 1.f, -1.f, 0.f, {0.f, 1.f, 0.f, 1.f} });
	this->vertices.push_back({ -1.f, -1.f, 0.f, {0.f, 0.f, 1.f, 1.f} });

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
	ID3DBlob* VertexShader, * PixelShader;
	D3DX11CompileFromFile("shader.fx", nullptr, NULL, "VertexMain", "vs_4_0", NULL, NULL, nullptr, &VertexShader, nullptr, nullptr);
	D3DX11CompileFromFile("shader.fx", nullptr, NULL, "PixelMain", "ps_4_0", NULL, NULL, nullptr, &PixelShader, nullptr, nullptr);

	/* Creation of our input layout */
	ComPtr<ID3D11InputLayout> iLayout;

	D3D11_INPUT_ELEMENT_DESC iLayoutDesc[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, NULL},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, NULL},
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

void Mesh::PreRender() {
	this->SetupBuffer();
}

void Mesh::Update() {
	UINT offset = 0;
	UINT stride = sizeof(vertex);
	/* We'll print triangles */
	this->con->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	this->con->IASetVertexBuffers(0, 1, this->buff.GetAddressOf(), &stride, &offset);

	/* The magic method. The ~DRAW~ */
	this->con->Draw(this->vertices.size(), 0);
}

void Mesh::LoadModel() {

}